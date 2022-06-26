#pragma once
#ifndef THREADCHECK_H
#define THREADCHECK_H

#include "Algorithm.h"
#include "ThreadPool.h"

extern bool threadCheck1TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);
extern bool threadCheck2TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);
extern bool threadCheck3TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);
extern bool threadCheck4TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);
extern bool threadCheck5TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);
extern bool threadCheck6TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src);


#endif
