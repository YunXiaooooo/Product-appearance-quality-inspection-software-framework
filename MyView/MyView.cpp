#include "MyView.h"
namespace View {
	MyView::MyView(QWidget *parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);

		connect(ui.actionImageSetting, SIGNAL(triggered()), this, SLOT(onActionImageSetting()));
		connect(ui.actionSignal, SIGNAL(triggered()), this, SLOT(onActionSignal()));

		mCamerasNum = Controller::DomainController::getInstance()->getCameraNums();		// 获取相机数量

		if (mCamerasNum > 0) {
			// 初始化根据相机数量初始化的ListWidgetCameras
			initListWidgetCameras();
			// 初始化订单管理相关数据
			initOrderMsg();
		}
	}

	MyView::~MyView() {

	}

	void MyView::onActionSignal() {
		std::cout << "get external signal." << std::endl;
		if (curRunModelName != "") {	// 选取目标模型

			// 更新抓取图像
			mCamerasMat.clear();
			mCamerasAfterProcessMat.clear();
			mCamerasMat.resize(mCamerasNum);
			mCamerasAfterProcessMat.resize(mCamerasNum);
			for (int i = 0; i < mCamerasNum; ++i) {
				Controller::DomainController::getInstance()->getSingleImage(i, mCamerasMat[i]);
				mCamerasAfterProcessMat[i] = mCamerasMat[i].clone();
			}
			// 获取处理结果
			singleProcessResult.clear();
			singleProcessResult = Controller::DomainController::getInstance()
				->taskRun(mCamerasMat, mCameraParamIndex, mBlockToCameraIndex, mBlockParam, mSettingParam);

			// 更新显示相关
			updateDisplay();
			// 更新数据相关
			updateOrderRelateMsgToDataBase();
		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Please set up the model first!"), QMessageBox::Yes);
		}
	}

	void MyView::updateOrderRelateMsgDisplay() {
		// 先更新显示，再入库
		mCurrentOrderMsg[3] = std::to_string(atoi(mCurrentOrderMsg[3].c_str()) + 1);	// Total

		mCurrentUserTargetOrderMsg[2] = std::to_string(atoi(mCurrentUserTargetOrderMsg[2].c_str()) + 1);
		mCurrentUserTargetOrderMsg[3] = std::to_string(atoi(mCurrentUserTargetOrderMsg[3].c_str()) - 1);

		bool isOK = true;
		for (int i = 0; i < singleProcessResult.size(); ++i) {
			if (!singleProcessResult[i]) {
				isOK = false;
				break;
			}
		}

		if (isOK) {
			mCurrentOrderMsg[4] = std::to_string(atoi(mCurrentOrderMsg[4].c_str()) + 1);	// 合格次数

			mCurrentUserTargetOrderMsg[4] = std::to_string(atoi(mCurrentUserTargetOrderMsg[4].c_str()) + 1);

			ui.labelSingleResultDisplay->setText("OK");
		}
		else {
			mCurrentOrderMsg[5] = std::to_string(atoi(mCurrentOrderMsg[5].c_str()) + 1);	// 不合格次数

			mCurrentUserTargetOrderMsg[5] = std::to_string(atoi(mCurrentUserTargetOrderMsg[5].c_str()) + 1);

			ui.labelSingleResultDisplay->setText("NG");
		}

		updateToRunDisplayMsg();
	}

	void MyView::updateOrderRelateMsgToDataBase() {
		// (TODO)更新后的数据入库，这边暂时先不弄，因为感觉表的参数还是有点模糊
	}

	void MyView::updateDisplay() {
		// 更新错误Item显示
		updateErrorItemDisplay();
		// 更新图像显示（保证画Block在最后一个动作，不影响其他显示）
		updateProcessBlockResult();
		// 统计数据更新
		updateOrderRelateMsgDisplay();
	}

	void MyView::updateProcessBlockResult() {
		for (int i = 0; i < singleProcessResult.size(); ++i) {
			if (singleProcessResult[i]) {
				cv::rectangle(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					cv::Point(mBlockParam[i][0], mBlockParam[i][1]),
					cv::Point(mBlockParam[i][2], mBlockParam[i][3]), CvScalar(0, 170, 0), mBlockRectDisplayThickness, cv::LINE_8, 0);

				cv::Size fontSize = cv::getTextSize(std::to_string(i), cv::FONT_HERSHEY_SIMPLEX, mBlockTextDisplayFontScale, mBlockTextDisplayFontThickness, 0);
				cv::rectangle(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					cv::Point(mBlockParam[i][0], mBlockParam[i][1] - fontSize.height),
					cv::Point(mBlockParam[i][0] + fontSize.width, mBlockParam[i][1]), CvScalar(0, 170, 0), cv::FILLED);
				cv::putText(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					std::to_string(i), cv::Point(mBlockParam[i][0], mBlockParam[i][1]), cv::FONT_HERSHEY_SIMPLEX, mBlockTextDisplayFontScale, (0, 0, 0), mBlockTextDisplayFontThickness);
			}
			else {
				cv::rectangle(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					cv::Point(mBlockParam[i][0], mBlockParam[i][1]),
					cv::Point(mBlockParam[i][2], mBlockParam[i][3]), CvScalar(0, 85, 255), mBlockRectDisplayThickness, cv::LINE_8, 0);

				cv::Size fontSize = cv::getTextSize(std::to_string(i), cv::FONT_HERSHEY_SIMPLEX, mBlockTextDisplayFontScale, mBlockTextDisplayFontThickness, 0);
				cv::rectangle(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					cv::Point(mBlockParam[i][0], mBlockParam[i][1] - fontSize.height),
					cv::Point(mBlockParam[i][0] + fontSize.width, mBlockParam[i][1]), CvScalar(0, 85, 255), cv::FILLED);
				cv::putText(mCamerasAfterProcessMat[mBlockToCameraIndex[i]],
					std::to_string(i), cv::Point(mBlockParam[i][0], mBlockParam[i][1]), cv::FONT_HERSHEY_SIMPLEX, mBlockTextDisplayFontScale, (0, 0, 0), mBlockTextDisplayFontThickness);

				// 判断哪个出错
				std::vector<int> thisTaskErrorCheckItem = Controller::DomainController::getInstance()->getTargetTaskErrorCheck(i);
				std::cout << "Task Error Index:[" << i << "]——[ ";
				for (int lineIdx = 0; lineIdx < thisTaskErrorCheckItem.size(); ++lineIdx) {
					std::cout << mDetectTypeSettingNames[thisTaskErrorCheckItem[lineIdx]] << " ";
					// (TODO)显示到界面上

				}
				std::wcout << "]" << std::endl;
			}
		}
		// 显示到ListWidgetCameras中
		for (int i = 0; i < mCamerasNum; ++i) {
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[i].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			mListWidgetCamerasItemWidget[i]->setSingleCameraItem(singleQImage);	// 设置图像显示
		}
		// 先每次都更新相机0到上面的大label中
		onListWidgetCamerasItemPressed(0);
	}

	void MyView::updateErrorItemDisplay() {
		// 只有当目前用于显示错误Item的个数超过当前最大的时候在更新，做一个类似延迟加载的方式
		int singleErrorCount = 0;	// 记录单次处理结果中的有问题区域的个数
		for (int i = 0; i < singleProcessResult.size(); ++i) {
			if (!singleProcessResult[i]) {
				// 判断是否超过当前mListWidgetErrorItem个数
				if (singleErrorCount >= mListWidgetErrorItemWidget.size()) {
					QListWidgetErrorItem* tmpErrorItem = new QListWidgetErrorItem(singleErrorCount, i, mBlockToCameraIndex[i]);	// 这是第几个错误的Item，第几个block，是第几个相机的
					mListWidgetErrorItemWidget.push_back(tmpErrorItem);

					QListWidgetItem* tmpListItem = new QListWidgetItem(ui.listWidgetError);
					mListWidgetErrorItem.push_back(tmpListItem);

					ui.listWidgetError->addItem(tmpListItem);
					tmpListItem->setSizeHint(listWidgetErrorItemWidgetSize);
					ui.listWidgetError->setItemWidget(tmpListItem, tmpErrorItem);
				}
				else {
					mListWidgetErrorItemWidget[singleErrorCount]->setErrorItemIndex(singleErrorCount);
					mListWidgetErrorItemWidget[singleErrorCount]->setErrorItemBlockIndex(i);
					mListWidgetErrorItemWidget[singleErrorCount]->setErrorItemCameraIndex(mBlockToCameraIndex[i]);
				}
				qApp->processEvents();	// 不在这里加这句，会出现刚new出item时label变小的情况

				// 设置图像
				cv::Mat toDisplayMat = mCamerasAfterProcessMat[mBlockToCameraIndex[i]]
				(cv::Range(mBlockParam[i][1], mBlockParam[i][3]), cv::Range(mBlockParam[i][0], mBlockParam[i][2]));
				cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
				QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.step, QImage::Format_RGB888);

				mListWidgetErrorItemWidget[singleErrorCount]->setErrorItemDisplayImage(singleQImage, singleQImage);

				// 错误计数+1
				singleErrorCount += 1;
			}
		}

		// 删除掉后面的
		while (singleErrorCount < mListWidgetErrorItemWidget.size()) {
			ui.listWidgetError->removeItemWidget(mListWidgetErrorItem.back());

			// 不delete掉会出错
			delete mListWidgetErrorItem.back();
			delete mListWidgetErrorItemWidget.back();

			mListWidgetErrorItem.pop_back();
			mListWidgetErrorItemWidget.pop_back();
		}
	}

	void MyView::initModelMsg() {
		// 清空
		mBlockParam.clear();
		mSettingParam.clear();
		mBlockToCameraIndex.clear();
		mCameraParamIndex.clear();

		// 重新读
		std::cout << "readTargetModelBlockParam start" << std::endl;
		mBlockParam = Controller::DomainController::getInstance()->readTargetModelBlockParam(curRunModelName);
		std::cout << "readTargetModelBlockParam end" << std::endl;

		std::cout << "readTargetModelSettingParam start" << std::endl;
		mSettingParam = Controller::DomainController::getInstance()->readTargetModelSettingParam(curRunModelName);
		std::cout << "readTargetModelSettingParam end" << std::endl;

		std::cout << "readTargetModelBlock2CameraIndexParam start" << std::endl;
		mBlockToCameraIndex = Controller::DomainController::getInstance()->readTargetModelBlock2CameraIndexParam(curRunModelName);
		std::cout << "readTargetModelBlock2CameraIndexParam end" << std::endl;

		std::cout << "readTargetModelCamera2BlockIndexParam start" << std::endl;
		mCameraParamIndex = Controller::DomainController::getInstance()->readTargetModelCamera2BlockIndexParam(curRunModelName);
		std::cout << "readTargetModelCamera2BlockIndexParam end" << std::endl;

		// 更新ROI个数
		targetROINum = mBlockParam.size();
		std::cout << "targetROINum: " << targetROINum << std::endl;

		// 根据block的个数初始化单次存储结果
		singleProcessResult.clear();
		singleProcessResult.resize(targetROINum);

		mDetectTypeSettingNames.clear();
		mDetectTypeSettingNames = Controller::DomainController::getInstance()->getDetectTypeSettingNames();
	}

	void MyView::onSignalCurRunModel(std::string modelName) {
		std::cout << "curRunModelName: " << modelName << std::endl;
		curRunModelName = modelName;

		// 每次模型名称变化之后都要去数据库内重新读取一下数据
		initModelMsg();

		// 每次模型变化之后初步抓取一下图像
		initCamerasMat();
	}

	void MyView::initCamerasMat() {
		mCamerasMat.clear();
		mCamerasAfterProcessMat.clear();

		mCamerasMat.resize(mCamerasNum);
		mCamerasAfterProcessMat.resize(mCamerasNum);
		for (int i = 0; i < mCamerasNum; ++i) {
			Controller::DomainController::getInstance()->getSingleImage(i, mCamerasMat[i]);
			mCamerasAfterProcessMat[i] = mCamerasMat[i].clone();

			// 显示到ListWidgetCameras中
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[i].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			mListWidgetCamerasItemWidget[i]->setSingleCameraItem(singleQImage);	// 设置图像显示
		}
	}

	void MyView::onActionImageSetting() {
		ImageSettingView* mImageSetting = new ImageSettingView();
		mImageSetting->setAttribute(Qt::WA_DeleteOnClose);
		mImageSetting->show();
		connect(mImageSetting, SIGNAL(signalCurRunModel(std::string)), this, SLOT(onSignalCurRunModel(std::string)));
	}

	void MyView::initListWidgetCameras() {
		for (int i = 0; i < mCamerasNum; ++i) {
			QListWidgetCamerasItem* tmpItemWidget = new QListWidgetCamerasItem(std::to_string(i));	// 必须以这个命名，点击信号根据这个转为的int发出
			QListWidgetItem* tmpListItem = new QListWidgetItem(ui.listWidgetCameras);
			ui.listWidgetCameras->addItem(tmpListItem);
			tmpListItem->setSizeHint(listWidgetCamerasItemWidgetSize);
			ui.listWidgetCameras->setItemWidget(tmpListItem, tmpItemWidget);
			// 记录地址，用于后续更新图像
			mListWidgetCamerasItemWidget.push_back(tmpItemWidget);
			// 连接用于响应点击信号
			connect(tmpItemWidget, SIGNAL(itemPressed(int)), this, SLOT(onListWidgetCamerasItemPressed(int)));
		}
	}

	void MyView::onListWidgetCamerasItemPressed(int itemIdx) {
		if (curRunModelName != "") {
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[itemIdx].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			// 显示到上方的label中
			ui.labelCameraDisplay->setPixmap(QPixmap::fromImage(singleQImage)
				.scaled(ui.labelCameraDisplay->width(), ui.labelCameraDisplay->height(), Qt::KeepAspectRatio));
			// 更新groupbox名称用于显示相机几
			ui.groupBoxCamera->setTitle(QStringLiteral("cammera") + QString::fromStdString(std::to_string(itemIdx)));
		}
		else {
			std::cout << "Have no choose Model" << std::endl;
		}
	}

	void MyView::closeEvent(QCloseEvent* event)
	{
		// 在退出窗口之前，发送信号显示
		emit signalClose();
	}

	void MyView::initOrderMsg() {
		// 初始化使用人员信息
		initUserMsg();

		// 加载订单
		connect(ui.buttonToLoadOrderNumber, SIGNAL(released()), this, SLOT(onButtonToLoadOrderNumber()));

		// 加载用户
		connect(ui.buttonToLoadUser, SIGNAL(released()), this, SLOT(onButtonToLoadUser()));

		connect(ui.buttonSureInLoadOrderNumber, SIGNAL(released()), this, SLOT(onButtonSureInLoadOrderNumber()));
	}

	void MyView::onButtonSureInLoadOrderNumber() {
		if (mCurrentOrderNumber != "") {
			// 先检查当前用户是否有这个订单的需求
			std::string mTempQueryUserID = mCurrentUser["userID"];
			std::vector<std::vector<std::string>> mQueryMsgSearchByUserID = Controller::DomainController::getInstance()->getOrderMsgSearchByUserID(mTempQueryUserID);

			int targetOrderNumberIndex = -1;
			for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
				std::string tmpOrderNum = mQueryMsgSearchByUserID[i][0];
				if (tmpOrderNum == mCurrentOrderNumber) {
					targetOrderNumberIndex = i;
					break;
				}
			}

			if (targetOrderNumberIndex != -1) {	// 有这个任务
				// 记录当前这个User的这个Order的相关数据信息
				mCurrentUserTargetOrderMsg.clear();
				for (int j = 0; j < mQueryMsgSearchByUserID[targetOrderNumberIndex].size(); ++j) {
					mCurrentUserTargetOrderMsg.push_back(mQueryMsgSearchByUserID[targetOrderNumberIndex][j]);
				}
				// 去调取这个Order的整体信息
				mCurrentOrderMsg.clear();
				mCurrentOrderMsg = Controller::DomainController::getInstance()->getTargetOrderMsg(mCurrentOrderNumber);
				// 更新界面显示
				updateToRunDisplayMsg();
				QMessageBox::information(NULL, QStringLiteral("Note"), QStringLiteral("The order task information is loaded"), QMessageBox::Yes);
				ui.tabWidget->setCurrentIndex(0);
			}
			else {		// 这个userid没有这个ordernumber的任务
				QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("The current User ID does not have this order task, please contact the administrator to add"), QMessageBox::Yes);
			}

		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("The corresponding order number information is not loaded"), QMessageBox::Yes);
		}
	}

	void MyView::updateToRunDisplayMsg() {
		// 订单相关的信息显示
		try {
			ui.labelOrderNumber->setText(QString::fromStdString(mCurrentOrderMsg[0]));
			ui.labelCreateTime->setText(QString::fromStdString(mCurrentOrderMsg[1]));
			ui.labelOrderQuantity->setText(QString::fromStdString(mCurrentOrderMsg[2]));
			ui.labelAlreadyFinishQuantity->setText(QString::fromStdString(mCurrentOrderMsg[3]));
			ui.labelAlreadyOKQuantity->setText(QString::fromStdString(mCurrentOrderMsg[4]));
			ui.labelAlreadyNGQuantity->setText(QString::fromStdString(mCurrentOrderMsg[5]));
		}
		catch (const std::exception& e) {
			std::cout << "订单信息加载显示错误: " << e.what() << std::endl;
		}
		// 当前任务
		try {
			ui.labelUserUnFinishQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[3]));
			ui.labelNowFinishQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[2]));
			ui.labelNowOKQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[4]));
			ui.labelNowNGQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[5]));
		}
		catch (const std::exception& e) {
			std::cout << "任务信息显示错误: " << e.what() << std::endl;
		}
	}

	void MyView::initUserMsg() {
		// 获取UserCache里面的相关信息并显示到界面上
		mCurrentUser.clear();
		mCurrentUser = Controller::DomainController::getInstance()->getCurrentUser();
		// 显示
		try {
			ui.labelUserID->setText(QString::fromStdString(mCurrentUser["userID"]));
			ui.labelTrueName->setText(QString::fromStdString(mCurrentUser["userTrueName"]));
			ui.labelProductionLine->setText(QString::fromStdString(mCurrentUser["userProductionLine"]));

			QDateTime currentDateTime = QDateTime::currentDateTime();
			QString currentDateTimeStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss");
			ui.labelOnlineStartTime->setText(currentDateTimeStr);
		}
		catch (std::exception& e) {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Error obtaining user Cache information."), QMessageBox::Yes);
		}

	}

	void MyView::onButtonToLoadOrderNumber() {
		// 检查是否有这个订单
		std::string tmpTargetOrderNumber = ui.lineEditToLoadOrderNumber->text().toStdString();
		bool isFindTargetOrderNumber = Controller::DomainController::getInstance()->tryToLoadTargetOrder(tmpTargetOrderNumber);
		if (isFindTargetOrderNumber) {	// 存在这个订单，加载相关信息，包括实时显示tab的相关信息
			updateOrderMsgSearchByOrderNumber(tmpTargetOrderNumber);
		}
		else {
			// 只有权限级别为TechnicalStaff以上才可以添加用户
			bool canAddNewOrder = Controller::DomainController::getInstance()->checkAuthority("CommonStaff", mCurrentUser["userID"]);	// 检查权限
			if (canAddNewOrder) {	// 可以添加订单，技术人员以上
				// 是否添加此订单
				QMessageBox::StandardButton result =
					QMessageBox::question(NULL, QStringLiteral("Note"), QStringLiteral("Check if there is no such order, please add it"), QMessageBox::Yes | QMessageBox::No);
				if (result == QMessageBox::Yes)	// 添加当前订单
				{
					// 输入需要添加的当前订单数量
					bool getTargetOrderNumber = false;
					QString targetOrderQuantity = QInputDialog::getText(this, QStringLiteral("Note"),
						QStringLiteral("Please enter the product quantity of this order"), QLineEdit::Normal,
						"", &getTargetOrderNumber);
					bool getModel = false;
					QString targetModel = QInputDialog::getText(this, QStringLiteral("Note"),
						QStringLiteral("Please enter the Model name"), QLineEdit::Normal,
						"", &getModel);
					int model_id = -1;
					if (getModel && !targetModel.isEmpty())
					{
						std::string inputName(targetModel.toStdString());
						std::vector<std::string> names = Controller::DomainController::getInstance()->readAllModelNames();
						for (int i=0,length = names.size();i<length;++i)
						{
							if (names[i] == inputName)
							{
								model_id = i;
								break;
							}
						}
					}
					if (getTargetOrderNumber && !targetOrderQuantity.isEmpty() && model_id != -1) {
						// 在数据库中创建
						QDateTime currentDateTime = QDateTime::currentDateTime();
						QString currentDateTimeStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss");
						bool tryCreateNewOrder = Controller::DomainController::getInstance()->tryCreateNewOrder(tmpTargetOrderNumber, currentDateTimeStr.toStdString(), targetOrderQuantity.toStdString(), model_id);

						if (tryCreateNewOrder) {
							std::cout << "创建新订单成功" << std::endl;
							QMessageBox::information(NULL, QStringLiteral("Note"), QStringLiteral("Create new order successfully"), QMessageBox::Yes);
							// 查询显示相关信息
							updateOrderMsgSearchByOrderNumber(tmpTargetOrderNumber);
						}
						else {
							std::cout << "创建新订单失败" << std::endl;
							QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Failed to create new order"), QMessageBox::Yes);
						}
					}
					else {
						std::cout << "取消创建订单" << std::endl;
						QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Cancel the creation of this order"), QMessageBox::Yes);
					}
				}
			}
			else {	// 普通员工
				QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("No such order"), QMessageBox::Yes);
			}
		}
	}

	void MyView::updateOrderMsgSearchByOrderNumber(std::string orderNumber) {
		// 根据orderNumber查询数据
		mCurrentOrderNumber = orderNumber;
		mCurrentMsgSearchByOrderNum.clear();
		mCurrentMsgSearchByOrderNum = Controller::DomainController::getInstance()->getUsersMsgSearchByOrderNumber(orderNumber);
		// 更新显示
		ui.treeWidgetOrderNumberToUser->clear();
		ui.treeWidgetOrderNumberToUser->setColumnCount(3); //设置列数
		QStringList tmpHeaderList;
		tmpHeaderList << QStringLiteral("Task No. - ID") << QStringLiteral("Total") << QStringLiteral("Completed Quantity");
		ui.treeWidgetOrderNumberToUser->setHeaderLabels(tmpHeaderList); //设置标题头

		QStringList orderNumberItemList;
		orderNumberItemList << QString::fromStdString(orderNumber);
		int orderTotalNum = 0, orderHaveFinishNum = 0;
		for (int i = 0; i < mCurrentMsgSearchByOrderNum.size(); ++i) {
			orderTotalNum += atoi(mCurrentMsgSearchByOrderNum[i][1].c_str());
			orderHaveFinishNum += atoi(mCurrentMsgSearchByOrderNum[i][2].c_str());
		}
		orderNumberItemList << QString::fromStdString(std::to_string(orderTotalNum)) << QString::fromStdString(std::to_string(orderHaveFinishNum));

		QTreeWidgetItem* tmpOrderNumberTreeWidgetItem =
			new QTreeWidgetItem(ui.treeWidgetOrderNumberToUser, QStringList(orderNumberItemList));
		for (int i = 0; i < mCurrentMsgSearchByOrderNum.size(); ++i) {
			QStringList tmpUserItemList;
			tmpUserItemList << QString::fromStdString(mCurrentMsgSearchByOrderNum[i][0])
				<< QString::fromStdString(mCurrentMsgSearchByOrderNum[i][1])
				<< QString::fromStdString(mCurrentMsgSearchByOrderNum[i][2]);
			QTreeWidgetItem* tmpUserTreeWidgetItem =
				new QTreeWidgetItem(tmpOrderNumberTreeWidgetItem, QStringList(tmpUserItemList));

			tmpOrderNumberTreeWidgetItem->addChild(tmpUserTreeWidgetItem);
		}
		ui.treeWidgetOrderNumberToUser->addTopLevelItem(tmpOrderNumberTreeWidgetItem);
		ui.treeWidgetOrderNumberToUser->expandAll();

		// 添加进combox中
		disconnect(ui.comboBoxLoadOrderNumberUsers, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxLoadOrderNumberUsers(int)));
		ui.comboBoxLoadOrderNumberUsers->clear();
		for (int i = 0; i < mCurrentMsgSearchByOrderNum.size(); ++i) {
			ui.comboBoxLoadOrderNumberUsers->addItem(QString::fromStdString(mCurrentMsgSearchByOrderNum[i][0]));
		}
		// 最后总是添加一个用于对当前订单添加新用户的item
		ui.comboBoxLoadOrderNumberUsers->addItem("...");
		ui.comboBoxLoadOrderNumberUsers->setCurrentIndex(-1);
		connect(ui.comboBoxLoadOrderNumberUsers, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxLoadOrderNumberUsers(int)));
	}

	void MyView::onComboBoxLoadOrderNumberUsers(int idx) {
		if (idx != -1) {
			if (ui.comboBoxLoadOrderNumberUsers->count() == idx + 1) {	// 说明是最后一个，针对当前添加新用户
				// 只有权限级别为TechnicalStaff以上才可以添加用户
				bool canAddNewUser = Controller::DomainController::getInstance()->checkAuthority("CommonStaff", mCurrentUser["userID"]);	// 检查权限
				if (canAddNewUser) {	// 添加用户
					bool getTargetNewUser = false;
					QString targetNewUserID = QInputDialog::getText(this, QStringLiteral("Note"),
						QStringLiteral("Please enter the product task assignment user ID for this order"), QLineEdit::Normal,
						"", &getTargetNewUser);
					if (getTargetNewUser && !targetNewUserID.isEmpty()) {
						// 测试是否存在此用户
						if (Controller::DomainController::getInstance()->checkUserIsNewOne(targetNewUserID.toStdString())) {
							QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("This User ID is not available"), QMessageBox::Yes);
							return;
						}
						// 输入分配的任务数量
						bool getTargetNewUserQuantity = false;
						QString targetNewUserIDQuantity = QInputDialog::getText(this, QStringLiteral("Note"),
							QStringLiteral("Please enter the number of products assigned to this user ID for this order"), QLineEdit::Normal,
							"", &getTargetNewUserQuantity);
						if (getTargetNewUserQuantity && !targetNewUserIDQuantity.isEmpty()) {
							// 获取新用户的id与需要操作的数量
							bool isSuccessAddTaskForUser =
								Controller::DomainController::getInstance()->addOrderNewUserTask(mCurrentOrderNumber, targetNewUserID.toStdString(), targetNewUserIDQuantity.toStdString());
							if (!isSuccessAddTaskForUser) {
								QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Failed to add this order task"), QMessageBox::Yes);
							}
							else {
								updateOrderMsgSearchByOrderNumber(mCurrentOrderNumber);
							}
						}
						else {
							QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Failed to add this order task"), QMessageBox::Yes);
						}
					}
					else {
						QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Failed to add this order task"), QMessageBox::Yes);
					}
				}
				else {
					QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("No add user permission"), QMessageBox::Yes);
				}
			}
			else {	// 仅显示一下
				try {
					ui.lineEditLoadOrderNumberUserTargetQuantity->setText(QString::fromStdString(mCurrentMsgSearchByOrderNum[idx][1]));
					ui.lineEditLoadOrderNumberUserTargetUnFinishQuantity->setText(QString::fromStdString(mCurrentMsgSearchByOrderNum[idx][3]));
				}
				catch (const std::exception& e) {
					std::cout << "lineEdit数量显示失败: " << e.what() << std::endl;
				}
			}
		}
	}

	void MyView::onButtonToLoadUser() {
		// 检查是否有这个用户
		std::string tmpTargetUser = ui.lineEditToLoadUser->text().toStdString();
		bool isFindTargetUser = !Controller::DomainController::getInstance()->checkUserIsNewOne(tmpTargetUser);
		if (isFindTargetUser) {	// 存在这个用户
			updateOrderMsgSearchByUserID(tmpTargetUser);
		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("No such user"), QMessageBox::Yes);
		}
	}

	void MyView::updateOrderMsgSearchByUserID(std::string userID) {
		// 根据userID查询数据
		std::string mTempQueryUserID = userID;
		std::vector<std::vector<std::string>> mQueryMsgSearchByUserID = Controller::DomainController::getInstance()->getOrderMsgSearchByUserID(mTempQueryUserID);
		// 更新显示
		ui.treeWidgetUserToOrderNumber->clear();
		ui.treeWidgetUserToOrderNumber->setColumnCount(3); //设置列数
		QStringList tmpHeaderList;
		tmpHeaderList << QStringLiteral("ID - Task No.") << QStringLiteral("Total") << QStringLiteral("Completed Quantity");
		ui.treeWidgetUserToOrderNumber->setHeaderLabels(tmpHeaderList); //设置标题头

		QStringList userItemList;
		userItemList << QString::fromStdString(userID);
		int userTotalNum = 0, userHaveFinishNum = 0;
		for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
			userTotalNum += atoi(mQueryMsgSearchByUserID[i][1].c_str());
			userHaveFinishNum += atoi(mQueryMsgSearchByUserID[i][2].c_str());
		}
		userItemList << QString::fromStdString(std::to_string(userTotalNum)) << QString::fromStdString(std::to_string(userHaveFinishNum));

		QTreeWidgetItem *tmpUserIDTreeWidgetItem =
			new QTreeWidgetItem(ui.treeWidgetUserToOrderNumber, QStringList(userItemList));
		for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
			QStringList tmpOrderItemList;
			tmpOrderItemList << QString::fromStdString(mQueryMsgSearchByUserID[i][0])
				<< QString::fromStdString(mQueryMsgSearchByUserID[i][1])
				<< QString::fromStdString(mQueryMsgSearchByUserID[i][2]);
			QTreeWidgetItem *tmpOrderTreeWidgetItem =
				new QTreeWidgetItem(tmpUserIDTreeWidgetItem, QStringList(tmpOrderItemList));

			tmpUserIDTreeWidgetItem->addChild(tmpOrderTreeWidgetItem);
		}
		ui.treeWidgetUserToOrderNumber->addTopLevelItem(tmpUserIDTreeWidgetItem);
		ui.treeWidgetUserToOrderNumber->expandAll();

		// 更新combobox
		ui.comboBoxLoadUserOrderNumbers->clear();
		for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
			ui.comboBoxLoadUserOrderNumbers->addItem(QString::fromStdString(mQueryMsgSearchByUserID[i][0]));
		}
	}
}
