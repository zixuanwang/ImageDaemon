/*
 * SURFFeature.h
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#ifndef SURFFEATURE_H_
#define SURFFEATURE_H_
#include "GlobalConfig.h"
#include "Feature.h"
#include "HbaseAdapter.h"
#include "ImageResizer.h"

class SURFFeature: public Feature {
public:
	SURFFeature(int featureCount, int maxIter = 30);
	virtual ~SURFFeature();
	virtual void load(int64_t rowKey);
	virtual void save(int64_t rowKey);
	virtual void compute(const cv::Mat& image);
	virtual void add(const boost::shared_ptr<ANNTreeRoot>& pRoot, int64_t imageId);
	virtual void knnSearch(std::vector<Neighbor>* pNeighborArray,
			boost::shared_ptr<ANNTreeRoot> pRoot, int k);
	virtual void getFeature(std::vector<float>* pFeature,
			int index);
private:
	// Store the number of features per image. Usually this number is around GlobalConfig::SURF_FEATURE_COUNT_PER_IMAGE
	int mFeatureCount;
	int mMinFeatureCount;
	int mMaxFeatureCount;
	int mMaxIter;
	int mThreshold;
};

#endif /* SURFFEATURE_H_ */
