/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout;
    QLabel *label_8;
    QTabWidget *MapSetupTab;
    QWidget *tab;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *IntersectionXEdit;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_4;
    QLineEdit *IntersectionYEdit;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *AddIntersectionButton;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *FromIntersectionSpinBox;
    QLabel *label_2;
    QSpinBox *ToIntersectionSpinBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *AddConnectingRoadButton;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QSpinBox *IntersectionSpinBox;
    QLabel *label_6;
    QComboBox *ConSideComboBox;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *AddRoadButton;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSpinBox *ToRoadSpinBox;
    QCheckBox *InDirectionCheckBox;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *AddLanePushButton;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_9;
    QSlider *horizontalSlider;
    QLabel *LaneWidthLabel;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_9;
    QCheckBox *SnapToGridCheckBox;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_8;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_2;
    QPushButton *pushButton_7;
    QWidget *tab_2;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_8;
    QCheckBox *checkBox_2;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *lineEdit_3;
    QComboBox *comboBox_2;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_10;
    QLineEdit *lineEdit_4;
    QComboBox *comboBox_3;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_11;
    QLineEdit *lineEdit_5;
    QComboBox *comboBox_4;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *pushButton_12;
    QLabel *label_10;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_19;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_17;
    QSpacerItem *horizontalSpacer_17;
    QPushButton *pushButton_18;
    QPushButton *pushButton_19;
    QPushButton *pushButton_20;
    QFrame *SimulatorFrame;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1187, 653);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        MainWindow->setMouseTracking(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_5 = new QGridLayout(centralwidget);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(16777215, 30));
        QFont font;
        font.setPointSize(25);
        font.setBold(true);
        font.setWeight(75);
        label_8->setFont(font);
        label_8->setAcceptDrops(false);
        label_8->setIndent(20);

        verticalLayout->addWidget(label_8);

        MapSetupTab = new QTabWidget(centralwidget);
        MapSetupTab->setObjectName(QString::fromUtf8("MapSetupTab"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(MapSetupTab->sizePolicy().hasHeightForWidth());
        MapSetupTab->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 10, -1, -1);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_3);

        IntersectionXEdit = new QLineEdit(tab);
        IntersectionXEdit->setObjectName(QString::fromUtf8("IntersectionXEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(IntersectionXEdit->sizePolicy().hasHeightForWidth());
        IntersectionXEdit->setSizePolicy(sizePolicy3);
        IntersectionXEdit->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(IntersectionXEdit);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_4);

        IntersectionYEdit = new QLineEdit(tab);
        IntersectionYEdit->setObjectName(QString::fromUtf8("IntersectionYEdit"));
        sizePolicy3.setHeightForWidth(IntersectionYEdit->sizePolicy().hasHeightForWidth());
        IntersectionYEdit->setSizePolicy(sizePolicy3);
        IntersectionYEdit->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(IntersectionYEdit);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        AddIntersectionButton = new QPushButton(tab);
        AddIntersectionButton->setObjectName(QString::fromUtf8("AddIntersectionButton"));

        horizontalLayout_4->addWidget(AddIntersectionButton);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 10, -1, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        FromIntersectionSpinBox = new QSpinBox(tab);
        FromIntersectionSpinBox->setObjectName(QString::fromUtf8("FromIntersectionSpinBox"));

        horizontalLayout_2->addWidget(FromIntersectionSpinBox);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        ToIntersectionSpinBox = new QSpinBox(tab);
        ToIntersectionSpinBox->setObjectName(QString::fromUtf8("ToIntersectionSpinBox"));

        horizontalLayout_2->addWidget(ToIntersectionSpinBox);


        horizontalLayout_5->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        AddConnectingRoadButton = new QPushButton(tab);
        AddConnectingRoadButton->setObjectName(QString::fromUtf8("AddConnectingRoadButton"));

        horizontalLayout_5->addWidget(AddConnectingRoadButton);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 10, -1, -1);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_7->addWidget(label_5);

        IntersectionSpinBox = new QSpinBox(tab);
        IntersectionSpinBox->setObjectName(QString::fromUtf8("IntersectionSpinBox"));

        horizontalLayout_7->addWidget(IntersectionSpinBox);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        ConSideComboBox = new QComboBox(tab);
        ConSideComboBox->addItem(QString());
        ConSideComboBox->addItem(QString());
        ConSideComboBox->addItem(QString());
        ConSideComboBox->addItem(QString());
        ConSideComboBox->setObjectName(QString::fromUtf8("ConSideComboBox"));

        horizontalLayout_7->addWidget(ConSideComboBox);


        horizontalLayout_6->addLayout(horizontalLayout_7);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        AddRoadButton = new QPushButton(tab);
        AddRoadButton->setObjectName(QString::fromUtf8("AddRoadButton"));

        horizontalLayout_6->addWidget(AddRoadButton);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, 10, -1, -1);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_9->addWidget(label_7);

        ToRoadSpinBox = new QSpinBox(tab);
        ToRoadSpinBox->setObjectName(QString::fromUtf8("ToRoadSpinBox"));

        horizontalLayout_9->addWidget(ToRoadSpinBox);

        InDirectionCheckBox = new QCheckBox(tab);
        InDirectionCheckBox->setObjectName(QString::fromUtf8("InDirectionCheckBox"));

        horizontalLayout_9->addWidget(InDirectionCheckBox);


        horizontalLayout_8->addLayout(horizontalLayout_9);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        AddLanePushButton = new QPushButton(tab);
        AddLanePushButton->setObjectName(QString::fromUtf8("AddLanePushButton"));

        horizontalLayout_8->addWidget(AddLanePushButton);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_11->addWidget(label_9);

        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(horizontalSlider);

        LaneWidthLabel = new QLabel(tab);
        LaneWidthLabel->setObjectName(QString::fromUtf8("LaneWidthLabel"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        LaneWidthLabel->setFont(font1);

        horizontalLayout_11->addWidget(LaneWidthLabel);

        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_11->addWidget(comboBox);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_9);

        SnapToGridCheckBox = new QCheckBox(tab);
        SnapToGridCheckBox->setObjectName(QString::fromUtf8("SnapToGridCheckBox"));
        SnapToGridCheckBox->setChecked(true);

        horizontalLayout_11->addWidget(SnapToGridCheckBox);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_14);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        pushButton_8 = new QPushButton(tab);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        horizontalLayout_10->addWidget(pushButton_8);

        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_10->addWidget(pushButton_3);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);

        horizontalLayout_10->addWidget(pushButton_2);

        pushButton_7 = new QPushButton(tab);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setAutoDefault(false);

        horizontalLayout_10->addWidget(pushButton_7);


        verticalLayout_2->addLayout(horizontalLayout_10);


        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);

        MapSetupTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_6 = new QGridLayout(tab_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        checkBox_2 = new QCheckBox(tab_2);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setChecked(true);

        gridLayout_8->addWidget(checkBox_2, 1, 0, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_12->addWidget(label_11);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_7);

        lineEdit_3 = new QLineEdit(tab_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        sizePolicy3.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy3);
        lineEdit_3->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_12->addWidget(lineEdit_3);

        comboBox_2 = new QComboBox(tab_2);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        horizontalLayout_12->addWidget(comboBox_2);


        gridLayout_7->addLayout(horizontalLayout_12, 0, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_13->addWidget(label_12);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);

        lineEdit_4 = new QLineEdit(tab_2);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        sizePolicy3.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy3);
        lineEdit_4->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_13->addWidget(lineEdit_4);

        comboBox_3 = new QComboBox(tab_2);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        horizontalLayout_13->addWidget(comboBox_3);


        gridLayout_7->addLayout(horizontalLayout_13, 1, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_14->addWidget(label_13);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);

        lineEdit_5 = new QLineEdit(tab_2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        sizePolicy3.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy3);
        lineEdit_5->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_14->addWidget(lineEdit_5);

        comboBox_4 = new QComboBox(tab_2);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));

        horizontalLayout_14->addWidget(comboBox_4);


        gridLayout_7->addLayout(horizontalLayout_14, 2, 0, 1, 1);


        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        pushButton_9 = new QPushButton(tab_2);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));

        horizontalLayout_15->addWidget(pushButton_9);

        pushButton_10 = new QPushButton(tab_2);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));

        horizontalLayout_15->addWidget(pushButton_10);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_12);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_13);

        pushButton_12 = new QPushButton(tab_2);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setAutoDefault(false);

        horizontalLayout_15->addWidget(pushButton_12);


        gridLayout_8->addLayout(horizontalLayout_15, 2, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_8, 0, 0, 1, 1);

        MapSetupTab->addTab(tab_2, QString());

        verticalLayout->addWidget(MapSetupTab);

        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMaximumSize(QSize(16777215, 30));
        label_10->setFont(font);
        label_10->setAcceptDrops(false);
        label_10->setIndent(20);

        verticalLayout->addWidget(label_10);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        groupBox_2->setBaseSize(QSize(100, 100));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        gridLayout_2->addLayout(verticalLayout_5, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        label_19 = new QLabel(centralwidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMaximumSize(QSize(16777215, 30));
        label_19->setFont(font);
        label_19->setAcceptDrops(false);
        label_19->setIndent(20);

        verticalLayout->addWidget(label_19);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        groupBox_3->setMaximumSize(QSize(16777215, 100));
        groupBox_3->setBaseSize(QSize(100, 100));
        groupBox_3->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_17 = new QPushButton(groupBox_3);
        pushButton_17->setObjectName(QString::fromUtf8("pushButton_17"));

        horizontalLayout->addWidget(pushButton_17);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_17);

        pushButton_18 = new QPushButton(groupBox_3);
        pushButton_18->setObjectName(QString::fromUtf8("pushButton_18"));

        horizontalLayout->addWidget(pushButton_18);

        pushButton_19 = new QPushButton(groupBox_3);
        pushButton_19->setObjectName(QString::fromUtf8("pushButton_19"));

        horizontalLayout->addWidget(pushButton_19);

        pushButton_20 = new QPushButton(groupBox_3);
        pushButton_20->setObjectName(QString::fromUtf8("pushButton_20"));

        horizontalLayout->addWidget(pushButton_20);


        gridLayout_3->addLayout(horizontalLayout, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_3);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        SimulatorFrame = new QFrame(centralwidget);
        SimulatorFrame->setObjectName(QString::fromUtf8("SimulatorFrame"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(100);
        sizePolicy4.setVerticalStretch(100);
        sizePolicy4.setHeightForWidth(SimulatorFrame->sizePolicy().hasHeightForWidth());
        SimulatorFrame->setSizePolicy(sizePolicy4);
        SimulatorFrame->setMinimumSize(QSize(600, 600));
        SimulatorFrame->setFrameShape(QFrame::StyledPanel);
        SimulatorFrame->setFrameShadow(QFrame::Raised);

        gridLayout_4->addWidget(SimulatorFrame, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        MapSetupTab->setCurrentIndex(0);
        pushButton_7->setDefault(false);
        pushButton_12->setDefault(false);
        pushButton_17->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Setup", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "X Position:", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Y Position:", nullptr));
        AddIntersectionButton->setText(QApplication::translate("MainWindow", "Add Intersection", nullptr));
        label->setText(QApplication::translate("MainWindow", "From Intersection: ", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "To Intersection: ", nullptr));
        AddConnectingRoadButton->setText(QApplication::translate("MainWindow", "Add Connecting Road", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Intersection: ", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Connection Side:", nullptr));
        ConSideComboBox->setItemText(0, QApplication::translate("MainWindow", "Up", nullptr));
        ConSideComboBox->setItemText(1, QApplication::translate("MainWindow", "Right", nullptr));
        ConSideComboBox->setItemText(2, QApplication::translate("MainWindow", "Down", nullptr));
        ConSideComboBox->setItemText(3, QApplication::translate("MainWindow", "Left", nullptr));

        AddRoadButton->setText(QApplication::translate("MainWindow", "Add Road", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "To Road:", nullptr));
        InDirectionCheckBox->setText(QApplication::translate("MainWindow", "Is In road Direction", nullptr));
        AddLanePushButton->setText(QApplication::translate("MainWindow", "Add Lane", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Lane Width:", nullptr));
        LaneWidthLabel->setText(QApplication::translate("MainWindow", "100", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "m", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "cm", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "ft", nullptr));
        comboBox->setItemText(3, QApplication::translate("MainWindow", "inch", nullptr));
        comboBox->setItemText(4, QApplication::translate("MainWindow", "px", nullptr));

        SnapToGridCheckBox->setText(QApplication::translate("MainWindow", "Snap To Grid", nullptr));
        pushButton_8->setText(QApplication::translate("MainWindow", "Load Map", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Save Map", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Delete", nullptr));
        pushButton_7->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        MapSetupTab->setTabText(MapSetupTab->indexOf(tab), QApplication::translate("MainWindow", "Map", nullptr));
        checkBox_2->setText(QApplication::translate("MainWindow", "Multi Color", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Car Max Speed", nullptr));
        comboBox_2->setItemText(0, QApplication::translate("MainWindow", "km/h", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("MainWindow", "mph", nullptr));
        comboBox_2->setItemText(2, QApplication::translate("MainWindow", "m/s", nullptr));
        comboBox_2->setItemText(3, QApplication::translate("MainWindow", "ft/s", nullptr));

        label_12->setText(QApplication::translate("MainWindow", "Motorcycle Max Speed", nullptr));
        comboBox_3->setItemText(0, QApplication::translate("MainWindow", "km/h", nullptr));
        comboBox_3->setItemText(1, QApplication::translate("MainWindow", "mph", nullptr));
        comboBox_3->setItemText(2, QApplication::translate("MainWindow", "m/s", nullptr));
        comboBox_3->setItemText(3, QApplication::translate("MainWindow", "ft/s", nullptr));

        label_13->setText(QApplication::translate("MainWindow", "Truck Max Speed", nullptr));
        comboBox_4->setItemText(0, QApplication::translate("MainWindow", "km/h", nullptr));
        comboBox_4->setItemText(1, QApplication::translate("MainWindow", "mph", nullptr));
        comboBox_4->setItemText(2, QApplication::translate("MainWindow", "m/s", nullptr));
        comboBox_4->setItemText(3, QApplication::translate("MainWindow", "ft/s", nullptr));

        pushButton_9->setText(QApplication::translate("MainWindow", "Load Setup", nullptr));
        pushButton_10->setText(QApplication::translate("MainWindow", "Save Setup", nullptr));
        pushButton_12->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        MapSetupTab->setTabText(MapSetupTab->indexOf(tab_2), QApplication::translate("MainWindow", "Vehicles", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Data", nullptr));
        groupBox_2->setTitle(QString());
        label_19->setText(QApplication::translate("MainWindow", "Run", nullptr));
        groupBox_3->setTitle(QString());
        pushButton_17->setText(QApplication::translate("MainWindow", "Run Simulation", nullptr));
        pushButton_18->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        pushButton_19->setText(QApplication::translate("MainWindow", "Continue", nullptr));
        pushButton_20->setText(QApplication::translate("MainWindow", "Abort", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
