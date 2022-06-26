#include "MyView.h"
namespace View {
	MyView::MyView(QWidget *parent)
		: QMainWindow(parent)
	{
		ui.setupUi(this);

		connect(ui.actionImageSetting, SIGNAL(triggered()), this, SLOT(onActionImageSetting()));
		connect(ui.actionSignal, SIGNAL(triggered()), this, SLOT(onActionSignal()));

		mCamerasNum = Controller::DomainController::getInstance()->getCameraNums();		// ��ȡ�������

		if (mCamerasNum > 0) {
			// ��ʼ���������������ʼ����ListWidgetCameras
			initListWidgetCameras();
			// ��ʼ�����������������
			initOrderMsg();
		}
	}

	MyView::~MyView() {

	}

	void MyView::onActionSignal() {
		std::cout << "get external signal." << std::endl;
		if (curRunModelName != "") {	// ѡȡĿ��ģ��

			// ����ץȡͼ��
			mCamerasMat.clear();
			mCamerasAfterProcessMat.clear();
			mCamerasMat.resize(mCamerasNum);
			mCamerasAfterProcessMat.resize(mCamerasNum);
			for (int i = 0; i < mCamerasNum; ++i) {
				Controller::DomainController::getInstance()->getSingleImage(i, mCamerasMat[i]);
				mCamerasAfterProcessMat[i] = mCamerasMat[i].clone();
			}
			// ��ȡ�������
			singleProcessResult.clear();
			singleProcessResult = Controller::DomainController::getInstance()
				->taskRun(mCamerasMat, mCameraParamIndex, mBlockToCameraIndex, mBlockParam, mSettingParam);

			// ������ʾ���
			updateDisplay();
			// �����������
			updateOrderRelateMsgToDataBase();
		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Please set up the model first!"), QMessageBox::Yes);
		}
	}

	void MyView::updateOrderRelateMsgDisplay() {
		// �ȸ�����ʾ�������
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
			mCurrentOrderMsg[4] = std::to_string(atoi(mCurrentOrderMsg[4].c_str()) + 1);	// �ϸ����

			mCurrentUserTargetOrderMsg[4] = std::to_string(atoi(mCurrentUserTargetOrderMsg[4].c_str()) + 1);

			ui.labelSingleResultDisplay->setText("OK");
		}
		else {
			mCurrentOrderMsg[5] = std::to_string(atoi(mCurrentOrderMsg[5].c_str()) + 1);	// ���ϸ����

			mCurrentUserTargetOrderMsg[5] = std::to_string(atoi(mCurrentUserTargetOrderMsg[5].c_str()) + 1);

			ui.labelSingleResultDisplay->setText("NG");
		}

		updateToRunDisplayMsg();
	}

	void MyView::updateOrderRelateMsgToDataBase() {
		// (TODO)���º��������⣬�����ʱ�Ȳ�Ū����Ϊ�о����Ĳ��������е�ģ��
	}

	void MyView::updateDisplay() {
		// ���´���Item��ʾ
		updateErrorItemDisplay();
		// ����ͼ����ʾ����֤��Block�����һ����������Ӱ��������ʾ��
		updateProcessBlockResult();
		// ͳ�����ݸ���
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

				// �ж��ĸ�����
				std::vector<int> thisTaskErrorCheckItem = Controller::DomainController::getInstance()->getTargetTaskErrorCheck(i);
				std::cout << "Task Error Index:[" << i << "]����[ ";
				for (int lineIdx = 0; lineIdx < thisTaskErrorCheckItem.size(); ++lineIdx) {
					std::cout << mDetectTypeSettingNames[thisTaskErrorCheckItem[lineIdx]] << " ";
					// (TODO)��ʾ��������

				}
				std::wcout << "]" << std::endl;
			}
		}
		// ��ʾ��ListWidgetCameras��
		for (int i = 0; i < mCamerasNum; ++i) {
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[i].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			mListWidgetCamerasItemWidget[i]->setSingleCameraItem(singleQImage);	// ����ͼ����ʾ
		}
		// ��ÿ�ζ��������0������Ĵ�label��
		onListWidgetCamerasItemPressed(0);
	}

	void MyView::updateErrorItemDisplay() {
		// ֻ�е�Ŀǰ������ʾ����Item�ĸ���������ǰ����ʱ���ڸ��£���һ�������ӳټ��صķ�ʽ
		int singleErrorCount = 0;	// ��¼���δ�������е�����������ĸ���
		for (int i = 0; i < singleProcessResult.size(); ++i) {
			if (!singleProcessResult[i]) {
				// �ж��Ƿ񳬹���ǰmListWidgetErrorItem����
				if (singleErrorCount >= mListWidgetErrorItemWidget.size()) {
					QListWidgetErrorItem* tmpErrorItem = new QListWidgetErrorItem(singleErrorCount, i, mBlockToCameraIndex[i]);	// ���ǵڼ��������Item���ڼ���block���ǵڼ��������
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
				qApp->processEvents();	// �����������䣬����ָ�new��itemʱlabel��С�����

				// ����ͼ��
				cv::Mat toDisplayMat = mCamerasAfterProcessMat[mBlockToCameraIndex[i]]
				(cv::Range(mBlockParam[i][1], mBlockParam[i][3]), cv::Range(mBlockParam[i][0], mBlockParam[i][2]));
				cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
				QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.step, QImage::Format_RGB888);

				mListWidgetErrorItemWidget[singleErrorCount]->setErrorItemDisplayImage(singleQImage, singleQImage);

				// �������+1
				singleErrorCount += 1;
			}
		}

		// ɾ���������
		while (singleErrorCount < mListWidgetErrorItemWidget.size()) {
			ui.listWidgetError->removeItemWidget(mListWidgetErrorItem.back());

			// ��delete�������
			delete mListWidgetErrorItem.back();
			delete mListWidgetErrorItemWidget.back();

			mListWidgetErrorItem.pop_back();
			mListWidgetErrorItemWidget.pop_back();
		}
	}

	void MyView::initModelMsg() {
		// ���
		mBlockParam.clear();
		mSettingParam.clear();
		mBlockToCameraIndex.clear();
		mCameraParamIndex.clear();

		// ���¶�
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

		// ����ROI����
		targetROINum = mBlockParam.size();
		std::cout << "targetROINum: " << targetROINum << std::endl;

		// ����block�ĸ�����ʼ�����δ洢���
		singleProcessResult.clear();
		singleProcessResult.resize(targetROINum);

		mDetectTypeSettingNames.clear();
		mDetectTypeSettingNames = Controller::DomainController::getInstance()->getDetectTypeSettingNames();
	}

	void MyView::onSignalCurRunModel(std::string modelName) {
		std::cout << "curRunModelName: " << modelName << std::endl;
		curRunModelName = modelName;

		// ÿ��ģ�����Ʊ仯֮��Ҫȥ���ݿ������¶�ȡһ������
		initModelMsg();

		// ÿ��ģ�ͱ仯֮�����ץȡһ��ͼ��
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

			// ��ʾ��ListWidgetCameras��
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[i].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			mListWidgetCamerasItemWidget[i]->setSingleCameraItem(singleQImage);	// ����ͼ����ʾ
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
			QListWidgetCamerasItem* tmpItemWidget = new QListWidgetCamerasItem(std::to_string(i));	// �������������������źŸ������תΪ��int����
			QListWidgetItem* tmpListItem = new QListWidgetItem(ui.listWidgetCameras);
			ui.listWidgetCameras->addItem(tmpListItem);
			tmpListItem->setSizeHint(listWidgetCamerasItemWidgetSize);
			ui.listWidgetCameras->setItemWidget(tmpListItem, tmpItemWidget);
			// ��¼��ַ�����ں�������ͼ��
			mListWidgetCamerasItemWidget.push_back(tmpItemWidget);
			// ����������Ӧ����ź�
			connect(tmpItemWidget, SIGNAL(itemPressed(int)), this, SLOT(onListWidgetCamerasItemPressed(int)));
		}
	}

	void MyView::onListWidgetCamerasItemPressed(int itemIdx) {
		if (curRunModelName != "") {
			cv::Mat toDisplayMat = mCamerasAfterProcessMat[itemIdx].clone();
			cv::cvtColor(toDisplayMat, toDisplayMat, CV_BGR2RGB);
			cv::resize(toDisplayMat, toDisplayMat, cv::Size(toDisplayMat.cols / mDisplayScale, toDisplayMat.rows / mDisplayScale), 0, 0, cv::INTER_NEAREST);
			QImage singleQImage(toDisplayMat.data, toDisplayMat.cols, toDisplayMat.rows, toDisplayMat.cols * toDisplayMat.channels(), QImage::Format_RGB888);
			// ��ʾ���Ϸ���label��
			ui.labelCameraDisplay->setPixmap(QPixmap::fromImage(singleQImage)
				.scaled(ui.labelCameraDisplay->width(), ui.labelCameraDisplay->height(), Qt::KeepAspectRatio));
			// ����groupbox����������ʾ�����
			ui.groupBoxCamera->setTitle(QStringLiteral("cammera") + QString::fromStdString(std::to_string(itemIdx)));
		}
		else {
			std::cout << "Have no choose Model" << std::endl;
		}
	}

	void MyView::closeEvent(QCloseEvent* event)
	{
		// ���˳�����֮ǰ�������ź���ʾ
		emit signalClose();
	}

	void MyView::initOrderMsg() {
		// ��ʼ��ʹ����Ա��Ϣ
		initUserMsg();

		// ���ض���
		connect(ui.buttonToLoadOrderNumber, SIGNAL(released()), this, SLOT(onButtonToLoadOrderNumber()));

		// �����û�
		connect(ui.buttonToLoadUser, SIGNAL(released()), this, SLOT(onButtonToLoadUser()));

		connect(ui.buttonSureInLoadOrderNumber, SIGNAL(released()), this, SLOT(onButtonSureInLoadOrderNumber()));
	}

	void MyView::onButtonSureInLoadOrderNumber() {
		if (mCurrentOrderNumber != "") {
			// �ȼ�鵱ǰ�û��Ƿ����������������
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

			if (targetOrderNumberIndex != -1) {	// ���������
				// ��¼��ǰ���User�����Order�����������Ϣ
				mCurrentUserTargetOrderMsg.clear();
				for (int j = 0; j < mQueryMsgSearchByUserID[targetOrderNumberIndex].size(); ++j) {
					mCurrentUserTargetOrderMsg.push_back(mQueryMsgSearchByUserID[targetOrderNumberIndex][j]);
				}
				// ȥ��ȡ���Order��������Ϣ
				mCurrentOrderMsg.clear();
				mCurrentOrderMsg = Controller::DomainController::getInstance()->getTargetOrderMsg(mCurrentOrderNumber);
				// ���½�����ʾ
				updateToRunDisplayMsg();
				QMessageBox::information(NULL, QStringLiteral("Note"), QStringLiteral("The order task information is loaded"), QMessageBox::Yes);
				ui.tabWidget->setCurrentIndex(0);
			}
			else {		// ���useridû�����ordernumber������
				QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("The current User ID does not have this order task, please contact the administrator to add"), QMessageBox::Yes);
			}

		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("The corresponding order number information is not loaded"), QMessageBox::Yes);
		}
	}

	void MyView::updateToRunDisplayMsg() {
		// ������ص���Ϣ��ʾ
		try {
			ui.labelOrderNumber->setText(QString::fromStdString(mCurrentOrderMsg[0]));
			ui.labelCreateTime->setText(QString::fromStdString(mCurrentOrderMsg[1]));
			ui.labelOrderQuantity->setText(QString::fromStdString(mCurrentOrderMsg[2]));
			ui.labelAlreadyFinishQuantity->setText(QString::fromStdString(mCurrentOrderMsg[3]));
			ui.labelAlreadyOKQuantity->setText(QString::fromStdString(mCurrentOrderMsg[4]));
			ui.labelAlreadyNGQuantity->setText(QString::fromStdString(mCurrentOrderMsg[5]));
		}
		catch (const std::exception& e) {
			std::cout << "������Ϣ������ʾ����: " << e.what() << std::endl;
		}
		// ��ǰ����
		try {
			ui.labelUserUnFinishQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[3]));
			ui.labelNowFinishQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[2]));
			ui.labelNowOKQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[4]));
			ui.labelNowNGQuantity->setText(QString::fromStdString(mCurrentUserTargetOrderMsg[5]));
		}
		catch (const std::exception& e) {
			std::cout << "������Ϣ��ʾ����: " << e.what() << std::endl;
		}
	}

	void MyView::initUserMsg() {
		// ��ȡUserCache����������Ϣ����ʾ��������
		mCurrentUser.clear();
		mCurrentUser = Controller::DomainController::getInstance()->getCurrentUser();
		// ��ʾ
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
		// ����Ƿ����������
		std::string tmpTargetOrderNumber = ui.lineEditToLoadOrderNumber->text().toStdString();
		bool isFindTargetOrderNumber = Controller::DomainController::getInstance()->tryToLoadTargetOrder(tmpTargetOrderNumber);
		if (isFindTargetOrderNumber) {	// ����������������������Ϣ������ʵʱ��ʾtab�������Ϣ
			updateOrderMsgSearchByOrderNumber(tmpTargetOrderNumber);
		}
		else {
			// ֻ��Ȩ�޼���ΪTechnicalStaff���ϲſ��������û�
			bool canAddNewOrder = Controller::DomainController::getInstance()->checkAuthority("CommonStaff", mCurrentUser["userID"]);	// ���Ȩ��
			if (canAddNewOrder) {	// �������Ӷ�����������Ա����
				// �Ƿ����Ӵ˶���
				QMessageBox::StandardButton result =
					QMessageBox::question(NULL, QStringLiteral("Note"), QStringLiteral("Check if there is no such order, please add it"), QMessageBox::Yes | QMessageBox::No);
				if (result == QMessageBox::Yes)	// ���ӵ�ǰ����
				{
					// ������Ҫ���ӵĵ�ǰ��������
					bool getTargetOrderNumber = false;
					QString targetOrderQuantity = QInputDialog::getText(this, QStringLiteral("Note"),
						QStringLiteral("Please enter the product quantity of this order"), QLineEdit::Normal,
						"", &getTargetOrderNumber);
					if (getTargetOrderNumber && !targetOrderQuantity.isEmpty()) {
						// �����ݿ��д���
						QDateTime currentDateTime = QDateTime::currentDateTime();
						QString currentDateTimeStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss");
						bool tryCreateNewOrder = Controller::DomainController::getInstance()->tryCreateNewOrder(tmpTargetOrderNumber, currentDateTimeStr.toStdString(), targetOrderQuantity.toStdString());

						if (tryCreateNewOrder) {
							std::cout << "�����¶����ɹ�" << std::endl;
							QMessageBox::information(NULL, QStringLiteral("Note"), QStringLiteral("Create new order successfully"), QMessageBox::Yes);
							// ��ѯ��ʾ�����Ϣ
							updateOrderMsgSearchByOrderNumber(tmpTargetOrderNumber);
						}
						else {
							std::cout << "�����¶���ʧ��" << std::endl;
							QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Failed to create new order"), QMessageBox::Yes);
						}
					}
					else {
						std::cout << "ȡ����������" << std::endl;
						QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("Cancel the creation of this order"), QMessageBox::Yes);
					}
				}
			}
			else {	// ��ͨԱ��
				QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("No such order"), QMessageBox::Yes);
			}
		}
	}

	void MyView::updateOrderMsgSearchByOrderNumber(std::string orderNumber) {
		// ����orderNumber��ѯ����
		mCurrentOrderNumber = orderNumber;
		mCurrentMsgSearchByOrderNum.clear();
		mCurrentMsgSearchByOrderNum = Controller::DomainController::getInstance()->getUsersMsgSearchByOrderNumber(orderNumber);
		// ������ʾ
		ui.treeWidgetOrderNumberToUser->clear();
		ui.treeWidgetOrderNumberToUser->setColumnCount(3); //��������
		QStringList tmpHeaderList;
		tmpHeaderList << QStringLiteral("Order No. - ID") << QStringLiteral("Total") << QStringLiteral("Completed Quantity");
		ui.treeWidgetOrderNumberToUser->setHeaderLabels(tmpHeaderList); //���ñ���ͷ

		QStringList orderNumberItemList;
		orderNumberItemList << QString::fromStdString(orderNumber);
		int orderTotalNum = 0, orderHaveFinishNum = 0;
		for (int i = 0; i < mCurrentMsgSearchByOrderNum.size(); ++i) {
			orderTotalNum += atoi(mCurrentMsgSearchByOrderNum[0][1].c_str());
			orderHaveFinishNum += atoi(mCurrentMsgSearchByOrderNum[0][2].c_str());
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

		// ���ӽ�combox��
		disconnect(ui.comboBoxLoadOrderNumberUsers, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxLoadOrderNumberUsers(int)));
		ui.comboBoxLoadOrderNumberUsers->clear();
		for (int i = 0; i < mCurrentMsgSearchByOrderNum.size(); ++i) {
			ui.comboBoxLoadOrderNumberUsers->addItem(QString::fromStdString(mCurrentMsgSearchByOrderNum[i][0]));
		}
		// �����������һ�����ڶԵ�ǰ�����������û���item
		ui.comboBoxLoadOrderNumberUsers->addItem("...");
		ui.comboBoxLoadOrderNumberUsers->setCurrentIndex(-1);
		connect(ui.comboBoxLoadOrderNumberUsers, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxLoadOrderNumberUsers(int)));
	}

	void MyView::onComboBoxLoadOrderNumberUsers(int idx) {
		if (idx != -1) {
			if (ui.comboBoxLoadOrderNumberUsers->count() == idx + 1) {	// ˵�������һ������Ե�ǰ�������û�
				// ֻ��Ȩ�޼���ΪTechnicalStaff���ϲſ��������û�
				bool canAddNewUser = Controller::DomainController::getInstance()->checkAuthority("CommonStaff", mCurrentUser["userID"]);	// ���Ȩ��
				if (canAddNewUser) {	// �����û�
					bool getTargetNewUser = false;
					QString targetNewUserID = QInputDialog::getText(this, QStringLiteral("Note"),
						QStringLiteral("Please enter the product task assignment user ID for this order"), QLineEdit::Normal,
						"", &getTargetNewUser);
					if (getTargetNewUser && !targetNewUserID.isEmpty()) {
						// �����Ƿ���ڴ��û�
						if (Controller::DomainController::getInstance()->checkUserIsNewOne(targetNewUserID.toStdString())) {
							QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("This User ID is not available"), QMessageBox::Yes);
							return;
						}
						// ����������������
						bool getTargetNewUserQuantity = false;
						QString targetNewUserIDQuantity = QInputDialog::getText(this, QStringLiteral("Note"),
							QStringLiteral("Please enter the number of products assigned to this user ID for this order"), QLineEdit::Normal,
							"", &getTargetNewUserQuantity);
						if (getTargetNewUserQuantity && !targetNewUserIDQuantity.isEmpty()) {
							// ��ȡ���û���id����Ҫ����������
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
			else {	// ����ʾһ��
				try {
					ui.lineEditLoadOrderNumberUserTargetQuantity->setText(QString::fromStdString(mCurrentMsgSearchByOrderNum[idx][1]));
					ui.lineEditLoadOrderNumberUserTargetUnFinishQuantity->setText(QString::fromStdString(mCurrentMsgSearchByOrderNum[idx][3]));
				}
				catch (const std::exception& e) {
					std::cout << "lineEdit������ʾʧ��: " << e.what() << std::endl;
				}
			}
		}
	}

	void MyView::onButtonToLoadUser() {
		// ����Ƿ�������û�
		std::string tmpTargetUser = ui.lineEditToLoadUser->text().toStdString();
		bool isFindTargetUser = !Controller::DomainController::getInstance()->checkUserIsNewOne(tmpTargetUser);
		if (isFindTargetUser) {	// ��������û�
			updateOrderMsgSearchByUserID(tmpTargetUser);
		}
		else {
			QMessageBox::warning(NULL, QStringLiteral("warning"), QStringLiteral("No such user"), QMessageBox::Yes);
		}
	}

	void MyView::updateOrderMsgSearchByUserID(std::string userID) {
		// ����userID��ѯ����
		std::string mTempQueryUserID = userID;
		std::vector<std::vector<std::string>> mQueryMsgSearchByUserID = Controller::DomainController::getInstance()->getOrderMsgSearchByUserID(mTempQueryUserID);
		// ������ʾ
		ui.treeWidgetUserToOrderNumber->clear();
		ui.treeWidgetUserToOrderNumber->setColumnCount(3); //��������
		QStringList tmpHeaderList;
		tmpHeaderList << QStringLiteral("ID - Order No.") << QStringLiteral("Total") << QStringLiteral("Completed Quantity");
		ui.treeWidgetUserToOrderNumber->setHeaderLabels(tmpHeaderList); //���ñ���ͷ

		QStringList userItemList;
		userItemList << QString::fromStdString(userID);
		int userTotalNum = 0, userHaveFinishNum = 0;
		for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
			userTotalNum += atoi(mQueryMsgSearchByUserID[0][1].c_str());
			userHaveFinishNum += atoi(mQueryMsgSearchByUserID[0][2].c_str());
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

		// ����combobox
		ui.comboBoxLoadUserOrderNumbers->clear();
		for (int i = 0; i < mQueryMsgSearchByUserID.size(); ++i) {
			ui.comboBoxLoadUserOrderNumbers->addItem(QString::fromStdString(mQueryMsgSearchByUserID[i][0]));
		}
	}
}