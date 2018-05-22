#pragma once

#include <ccPointCloud.h>

#include <pdal/pdal.hpp>
#include <GreyhoundCommon.hpp>

#include "PDALConverter.h"

std::unique_ptr<ccPointCloud> download_and_convert_cloud(pdal::Options opts, PDALConverter converter = PDALConverter());

struct BoundsDepth
{
	pdal::greyhound::Bounds b;
	int depth;
};

class GreyhoundDownloader
{
public:
	enum DOWNLOAD_METHOD
	{
		DEPTH_BY_DEPTH,
		QUADTREE,
		OCTREE
	};

public:
	GreyhoundDownloader(pdal::Options opts, uint32_t start_depth, pdal::greyhound::Bounds bounds, PDALConverter converter);
	void download_to(ccPointCloud* cloud, DOWNLOAD_METHOD, std::function<void()> cb);


private:
	pdal::Options m_opts;
	uint32_t m_current_depth;
	pdal::greyhound::Bounds m_bounds;
	PDALConverter m_converter;
};