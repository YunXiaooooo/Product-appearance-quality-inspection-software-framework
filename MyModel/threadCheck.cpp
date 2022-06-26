#include "threadCheck.h"

bool threadCheck1TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check1TestRegularRunningProcess(src);
}

bool threadCheck2TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check2TestRegularRunningProcess(src);
}

bool threadCheck3TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check3TestRegularRunningProcess(src);
}

bool threadCheck4TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check4TestRegularRunningProcess(src);
}

bool threadCheck5TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check5TestRegularRunningProcess(src);
}

bool threadCheck6TestRegularRunningProcess(Model::AlgorithmInterface* AI, cv::Mat& src)
{
    return AI->Check6TestRegularRunningProcess(src);
}

//bool threadCheck2TestRegularRunningProcess(cv::Mat& src)
//{
//    DetectTypeProcess* mDetectTypeProcess = new DetectTypeProcess;
//    bool result = true;
//    result = mDetectTypeProcess->Check2TestRegularRunningProcess(src);
//
//    delete mDetectTypeProcess;
//    return result;
//}
//bool threadCheck3TestRegularRunningProcess(cv::Mat& src)
//{
//    DetectTypeProcess* mDetectTypeProcess = new DetectTypeProcess;
//    bool result = true;
//    result = mDetectTypeProcess->Check3TestRegularRunningProcess(src);
//
//    delete mDetectTypeProcess;
//    return result;
//}
//
//bool threadCheck4TestRegularRunningProcess(cv::Mat& src)
//{
//    DetectTypeProcess* mDetectTypeProcess = new DetectTypeProcess;
//    bool result = true;
//    result = mDetectTypeProcess->Check4TestRegularRunningProcess(src);
//
//    delete mDetectTypeProcess;
//    return result;
//}
//bool threadCheck5TestRegularRunningProcess(cv::Mat& src)
//{
//    DetectTypeProcess* mDetectTypeProcess = new DetectTypeProcess;
//    bool result = true;
//    result = mDetectTypeProcess->Check5TestRegularRunningProcess(src);
//
//    delete mDetectTypeProcess;
//    return result;
//}
//bool threadCheck6TestRegularRunningProcess(cv::Mat& src)
//{
//    DetectTypeProcess* mDetectTypeProcess = new DetectTypeProcess;
//    bool result = true;
//    result = mDetectTypeProcess->Check6TestRegularRunningProcess(src);
//
//    delete mDetectTypeProcess;
//    return result;
//}
