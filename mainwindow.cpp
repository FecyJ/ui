#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 初始化控制状态栏
    controlStatusLabel = new QLabel("手柄操作状态: 就绪");
    ui->statusbar->addPermanentWidget(controlStatusLabel);
    
    // 连接信号槽
    // 模式切换
    connect(ui->manualModeButton, &QPushButton::clicked, this, &MainWindow::onManualModeButtonClicked);
    connect(ui->autoModeButton, &QPushButton::clicked, this, &MainWindow::onAutoModeButtonClicked);
    connect(ui->semiautoModeButton, &QPushButton::clicked, this, &MainWindow::onSemiautoModeButtonClicked);
    
    // 位置姿态控制
    connect(ui->cartesianButton, &QPushButton::clicked, this, &MainWindow::onCartesianButtonClicked);
    connect(ui->ptpButton, &QPushButton::clicked, this, &MainWindow::onPtpButtonClicked);
    
    // 手柄控制
    connect(ui->joystickEnableCheckBox, &QCheckBox::clicked, this, &MainWindow::onJoystickEnableCheckBoxClicked);
    connect(ui->zForwardButton, &QPushButton::pressed, this, &MainWindow::onZForwardButtonPressed);
    connect(ui->zForwardButton, &QPushButton::released, this, &MainWindow::onZForwardButtonReleased);
    connect(ui->zBackwardButton, &QPushButton::pressed, this, &MainWindow::onZBackwardButtonPressed);
    connect(ui->zBackwardButton, &QPushButton::released, this, &MainWindow::onZBackwardButtonReleased);
    
    // 参数调节
    connect(ui->speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedSliderValueChanged);
    connect(ui->sensitivitySlider, &QSlider::valueChanged, this, &MainWindow::onSensitivitySliderValueChanged);
    
    // 安全设置
    connect(ui->emergencyButton, &QPushButton::clicked, this, &MainWindow::onEmergencyButtonClicked);
    connect(ui->collisionCheckBox, &QCheckBox::clicked, this, &MainWindow::onCollisionCheckBoxClicked);
    
    // 视觉识别
    connect(ui->nutRecognitionButton, &QPushButton::clicked, this, &MainWindow::onNutRecognitionButtonClicked);
    connect(ui->boltRecognitionButton, &QPushButton::clicked, this, &MainWindow::onBoltRecognitionButtonClicked);
    connect(ui->objectRecognitionButton, &QPushButton::clicked, this, &MainWindow::onObjectRecognitionButtonClicked);
    connect(ui->prevTargetButton, &QPushButton::clicked, this, &MainWindow::onPrevTargetButtonClicked);
    connect(ui->nextTargetButton, &QPushButton::clicked, this, &MainWindow::onNextTargetButtonClicked);
    connect(ui->alignButton, &QPushButton::clicked, this, &MainWindow::onAlignButtonClicked);
    connect(ui->captureButton, &QPushButton::clicked, this, &MainWindow::onCaptureButtonClicked);
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::onRecordButtonClicked);
    connect(ui->cameraSourceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onCameraSourceComboBoxCurrentIndexChanged);
    
    // 模拟数据更新
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
    timer->start(1000); // 每秒更新一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 位置姿态控制槽函数
void MainWindow::onCartesianButtonClicked()
{
    qDebug() << "直线运动 (Cartesian) 按钮点击";
    ui->statusbar->showMessage("执行笛卡尔直线路径规划...", 2000);
}

void MainWindow::onPtpButtonClicked()
{
    qDebug() << "关节规划 (PTP) 按钮点击";
    ui->statusbar->showMessage("执行关节空间路径规划...", 2000);
}

// 手柄控制槽函数
void MainWindow::onJoystickEnableCheckBoxClicked(bool checked)
{
    qDebug() << "手柄激活状态: " << checked;
    ui->statusbar->showMessage(checked ? "手柄已激活" : "手柄已禁用", 2000);
    ui->controlStatusLabel->setText(checked ? "手柄操作状态: 已激活" : "手柄操作状态: 就绪");
}

void MainWindow::onZForwardButtonPressed()
{
    qDebug() << "前进 (LT) 按钮按下";
    ui->statusbar->showMessage("Z轴前进...", 1000);
}

void MainWindow::onZForwardButtonReleased()
{
    qDebug() << "前进 (LT) 按钮释放";
    ui->statusbar->showMessage("Z轴停止", 1000);
}

void MainWindow::onZBackwardButtonPressed()
{
    qDebug() << "后退 (RT) 按钮按下";
    ui->statusbar->showMessage("Z轴后退...", 1000);
}

void MainWindow::onZBackwardButtonReleased()
{
    qDebug() << "后退 (RT) 按钮释放";
    ui->statusbar->showMessage("Z轴停止", 1000);
}

// 视觉识别槽函数
void MainWindow::onAlignButtonClicked()
{
    qDebug() << "执行对准按钮点击";
    ui->statusbar->showMessage("正在执行视觉对准...", 2000);
    ui->recognitionResultLabel->setText("正在执行对准...");
}

// 模式切换槽函数
void MainWindow::onManualModeButtonClicked()
{
    qDebug() << "手动模式按钮点击";
    ui->statusbar->showMessage("切换到手动模式", 2000);
    ui->systemStatusLabel->setText("系统状态: 手动模式");
}

void MainWindow::onAutoModeButtonClicked()
{
    qDebug() << "自动模式按钮点击";
    ui->statusbar->showMessage("切换到自动模式", 2000);
    ui->systemStatusLabel->setText("系统状态: 自动模式");
}

void MainWindow::onSemiautoModeButtonClicked()
{
    qDebug() << "半自动模式按钮点击";
    ui->statusbar->showMessage("切换到半自动模式", 2000);
    ui->systemStatusLabel->setText("系统状态: 半自动模式");
}

// 参数调节槽函数
void MainWindow::onSpeedSliderValueChanged(int value)
{
    qDebug() << "速度限制调整为: " << value;
    ui->statusbar->showMessage(QString("速度限制: %1%").arg(value), 1000);
}

void MainWindow::onSensitivitySliderValueChanged(int value)
{
    qDebug() << "灵敏度调整为: " << value;
    ui->statusbar->showMessage(QString("灵敏度: %1%").arg(value), 1000);
}

// 安全设置槽函数
void MainWindow::onEmergencyButtonClicked()
{
    qDebug() << "紧急停止按钮点击";
    ui->statusbar->showMessage("紧急停止激活", 2000);
    ui->controlStatusLabel->setText("手柄操作状态: 紧急停止");
    ui->systemStatusLabel->setText("系统状态: 紧急停止");
}

void MainWindow::onCollisionCheckBoxClicked(bool checked)
{
    qDebug() << "碰撞检测状态: " << checked;
    ui->statusbar->showMessage(checked ? "碰撞检测已启用" : "碰撞检测已禁用", 2000);
}

// 视觉识别槽函数
void MainWindow::onNutRecognitionButtonClicked()
{
    qDebug() << "螺母识别模式按钮点击";
    ui->statusbar->showMessage("切换到螺母识别模式", 2000);
    ui->recognitionResultLabel->setText("识别模式: 螺母识别");
}

void MainWindow::onBoltRecognitionButtonClicked()
{
    qDebug() << "螺栓识别模式按钮点击";
    ui->statusbar->showMessage("切换到螺栓识别模式", 2000);
    ui->recognitionResultLabel->setText("识别模式: 螺栓识别");
}

void MainWindow::onObjectRecognitionButtonClicked()
{
    qDebug() << "通用物体识别模式按钮点击";
    ui->statusbar->showMessage("切换到通用物体识别模式", 2000);
    ui->recognitionResultLabel->setText("识别模式: 通用物体识别");
}

void MainWindow::onPrevTargetButtonClicked()
{
    qDebug() << "上一个目标按钮点击";
    ui->statusbar->showMessage("选择上一个目标", 1000);
    ui->targetIndexLabel->setText("目标 1/3");
}

void MainWindow::onNextTargetButtonClicked()
{
    qDebug() << "下一个目标按钮点击";
    ui->statusbar->showMessage("选择下一个目标", 1000);
    ui->targetIndexLabel->setText("目标 2/3");
}

void MainWindow::onCaptureButtonClicked()
{
    qDebug() << "截图按钮点击";
    ui->statusbar->showMessage("正在截图...", 1000);
    ui->recognitionResultLabel->setText("截图已保存");
}

void MainWindow::onRecordButtonClicked()
{
    qDebug() << "录制按钮点击";
    ui->statusbar->showMessage("开始录制...", 1000);
    ui->recognitionResultLabel->setText("录制中...");
}

void MainWindow::onCameraSourceComboBoxCurrentIndexChanged(int index)
{
    QString source = ui->cameraSourceComboBox->currentText();
    qDebug() << "相机源切换为: " << source;
    ui->statusbar->showMessage(QString("相机源: %1").arg(source), 1000);
}

// 状态更新槽函数
void MainWindow::updateStatusBar()
{
    // 模拟控制状态更新
    static QString statuses[] = {"就绪", "运行中", "接近奇异点", "碰撞警告"};
    static int statusIndex = 0;
    
    if (ui->joystickEnableCheckBox->isChecked()) {
        statusIndex = (statusIndex + 1) % 4;
        ui->controlStatusLabel->setText(QString("手柄操作状态: %1").arg(statuses[statusIndex]));
    }
}
