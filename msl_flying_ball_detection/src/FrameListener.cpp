/*
 * FrameListener.cpp
 *
 *  Created on: Jun 19, 2016
 *      Author: Stephan Opfer
 */

using namespace std;

#include "FrameListener.h"
#include <vector>
#include <OpenNI.h>
#include "OniSampleUtilities.h"

#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/ChannelFloat32.h>
#include <geometry_msgs/Point32.h>

#include <pcl-1.7/pcl/visualization/pcl_visualizer.h>
#include <pcl-1.7/pcl/console/parse.h>
#include <pcl-1.7/pcl/point_cloud.h>
#include <pcl-1.7/pcl/filters/extract_indices.h>
#include <pcl-1.7/pcl/io/pcd_io.h>
#include <pcl-1.7/pcl/point_types.h>
#include <pcl-1.7/pcl/segmentation/sac_segmentation.h>
#include <pcl-1.7/pcl/sample_consensus/sac.h>
#include <pcl-1.7/pcl/sample_consensus/ransac.h>
#include <pcl-1.7/pcl/sample_consensus/sac_model_sphere.h>
#include <pcl-1.7/pcl/sample_consensus/model_types.h>
#include <pcl-1.7/pcl/filters/voxel_grid.h>
#include <pcl-1.7/pcl/sample_consensus/method_types.h>
#include <pcl-1.7/pcl/kdtree/kdtree.h>

#include <pcl-1.7/pcl/ModelCoefficients.h>
#include <pcl-1.7/pcl/point_types.h>
#include <pcl-1.7/pcl/io/pcd_io.h>
#include <pcl-1.7/pcl/filters/extract_indices.h>
#include <pcl-1.7/pcl/filters/voxel_grid.h>
#include <pcl-1.7/pcl/features/normal_3d.h>
#include <pcl-1.7/pcl/kdtree/kdtree.h>
#include <pcl-1.7/pcl/sample_consensus/method_types.h>
#include <pcl-1.7/pcl/sample_consensus/model_types.h>
#include <pcl-1.7/pcl/segmentation/sac_segmentation.h>
#include <pcl-1.7/pcl/segmentation/extract_clusters.h>

namespace msl
{

	FrameListener::FrameListener()
	{
		pubBall = this->rosNode.advertise<sensor_msgs::PointCloud>("/astra/ball", 10000);
		pub = this->rosNode.advertise<sensor_msgs::PointCloud>("/astra/depthCloud", 10000);
	}

	FrameListener::~FrameListener()
	{
	}

	void FrameListener::onNewFrame(openni::VideoStream& vidStream)
	{
		// initialize PointClouds
		pcl::PointCloud<pcl::PointXYZ>::Ptr ballCloud(new pcl::PointCloud<pcl::PointXYZ>);
		pcl::PointCloud<pcl::PointXYZ>::Ptr rawCloud(new pcl::PointCloud<pcl::PointXYZ>);
		pcl::PointCloud<pcl::PointXYZ>::Ptr voxelCloud(new pcl::PointCloud<pcl::PointXYZ>);
//		pcl::PCDReader reader;
//		reader.read ("/home/emmeda/test_1.pcd", *rawCloud);
		rawCloud->width = vidStream.getVideoMode().getResolutionX();
		rawCloud->height = vidStream.getVideoMode().getResolutionY();
		rawCloud->is_dense = false;
		rawCloud->points.resize(rawCloud->width * rawCloud->height);

		// read depth image
		openni::VideoFrameRef depthFrame;
		vidStream.readFrame(&depthFrame);

		if (!depthFrame.isValid())
		{
			cout << "error! depthFrame is not valid!!!" << endl;
			return;
		}

		// Fill the raw Cloud with the depth image from the Astra S Pro
		float pDepthHist[vidStream.getMaxPixelValue()];
		calculateHistogram(pDepthHist, vidStream.getMaxPixelValue(), depthFrame);

		int rowSize = depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
		const openni::DepthPixel* depthPixel = (const openni::DepthPixel*)depthFrame.getData();
		int x, y = 0;

		for (int i = 0; i < rawCloud->points.size(); i++)
		{
			if (*depthPixel != 0)
			{
				x = i % rowSize;
				y = i / rowSize;
				float wX, wY, wZ;
				openni::CoordinateConverter::convertDepthToWorld(vidStream, y, x, *depthPixel, &wX, &wY, &wZ);

				// depth image comes within [mm], so make it to [m]
				rawCloud->points[i].x = wX / 1000.0;
				rawCloud->points[i].y = wY / 1000.0;
				rawCloud->points[i].z = wZ / 1000.0;
			}
			depthPixel++;
		}

		// Create the filtering object
		pcl::VoxelGrid<pcl::PointXYZ> sor;
		sor.setInputCloud(rawCloud);
		sor.setLeafSize(0.05f, 0.05f, 0.05f); // voxel size 5 cm
		sor.filter(*voxelCloud);
		this->publishCloud(voxelCloud, this->pub);

		// Creating the KdTree object for the search method of the extraction
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
		tree->setInputCloud(voxelCloud);

		std::vector<pcl::PointIndices> cluster_indices;
		pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
		ec.setClusterTolerance(0.08); // m
		ec.setMinClusterSize(20);
		ec.setMaxClusterSize(50);
		ec.setSearchMethod(tree);
		ec.setInputCloud(voxelCloud);
		ec.extract(cluster_indices);

		for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
		{
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
			for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); ++pit)
			{
				cloud_cluster->points.push_back(voxelCloud->points[*pit]);
			}
			cloud_cluster->width = cloud_cluster->points.size();
			cloud_cluster->height = 1;
			cloud_cluster->is_dense = true;

			Eigen::Vector4d centroid;
			pcl::compute3DCentroid(*cloud_cluster, centroid);

			bool ballFound = true;
			for (int i = 0; i < cloud_cluster->points.size(); i++)
			{
				if (sqrt((centroid.x() - cloud_cluster->points[i].x)*(centroid.x() - cloud_cluster->points[i].x)
					 +(centroid.y() - cloud_cluster->points[i].y)*(centroid.y() - cloud_cluster->points[i].y)
					 +(centroid.z() - cloud_cluster->points[i].z)*(centroid.z() - cloud_cluster->points[i].z))> 0.14) // some point is further away from the centroid than 14cm
				{
					ballFound = false;
					break;
				}
			}

			if (ballFound)
			{
				cout << "Ball is at: " << centroid.x() << ", " << centroid.y() << ", " << centroid.z() << endl;
				this->publishCloud(cloud_cluster, this->pubBall);
			}
		}
	}

	void FrameListener::publishCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr voxelCloud, ros::Publisher pub)
	{
		sensor_msgs::PointCloud pcl;
		vector<geometry_msgs::Point32> points;
		vector<sensor_msgs::ChannelFloat32> channel;
		sensor_msgs::ChannelFloat32 depthChannel;
		depthChannel.name = "depth";

		for (int i = 0; i < voxelCloud->points.size(); i++)
		{
			geometry_msgs::Point32 p;
			p.x = voxelCloud->points[i].x;
			p.y = voxelCloud->points[i].y;
			p.z = voxelCloud->points[i].z;
			points.push_back(p);
		}

		channel.push_back(depthChannel);
		pcl.channels = channel;
		pcl.points = points;
		pcl.header.frame_id = "camera_frame";
		pcl.header.stamp = ros::Time::now();
		pub.publish(pcl);
	}

} /* namespace msl */
