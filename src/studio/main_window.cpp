#include <QtWidgets>

#include "main_window.h"

#include <iostream>
#include "log.h"

// SDK includes
#include "frame_pack.h"
#include "camera_types.h"
#include "recording_types.h"
#include "input_data.h"

#include <QtWidgets>

MainWindow::MainWindow(QApplication* thisApp) :
    app(thisApp),
    m_viewMode(ViewMode::DEPTH),
    m_isRunning(false)
{
    Log::setWriter(m_sdk.m_logWriter);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Comet Studio"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow() {
    stop();
}

void MainWindow::play() {
    stop();
    Log::d("play", "STD");
    Log::d(dirView->selectedPath.c_str());
    bool initialized = m_sdk.initialize(CAMERA_KINECT2, dirView->selectedPath.c_str());
    if (initialized) {
        m_sdk.start();
        m_isRunning = true;
        m_sdkThread = std::thread(&MainWindow::run, this);
    }
}

void MainWindow::pause() {
    std::cout << "pause" << std::endl;
}

void MainWindow::stop() {
    m_sdk.stop();
    if (!m_isRunning) {
        return;
    }
    Log::d("stop", "STD");

    m_isRunning = false;
    if (m_sdkThread.joinable()) {
        m_sdkThread.join();
    }
}

void MainWindow::record() {
    bool status = m_sdk.getRecording();
    if (status == false) {
        int channels = RECORDING_NONE;
        channels = channels | RECORDING_DEPTH;
        channels = channels | RECORDING_IR;
        channels = channels | RECORDING_COLOR;
        channels = channels | RECORDING_COLOR_REG;
        m_sdk.setRecordingChannels(channels);
        m_sdk.setRecordingPath("C:\\Users\\Kuba\\Desktop\\Comet_Data\\2016.03.03_6.21_jl");
    }
    m_sdk.setRecording(!status);
    if (!status == true) {
        recordAct->setIcon(QIcon(":/icons/record_clicked.png"));
    } else {
        recordAct->setIcon(QIcon(":/icons/record.png"));
    }
}

void MainWindow::close() {
    QTimer::singleShot(250, app, SLOT(quit()));
}

void MainWindow::run() {
    while (m_isRunning){
        std::chrono::milliseconds time(15);
        std::this_thread::sleep_for(time);

        m_sdk.lock();
        if (m_sdk.m_newDataReady) {
            cm::Image img;
            switch (m_viewMode) {
                case ViewMode::DEPTH:
                    m_sdk.getColorDepth(img);
                    break;
                case ViewMode::IR:
                    m_sdk.getColorIr(img);
                    break;
                case ViewMode::COLOR:
                    m_sdk.getColor(img);
                    break;
                case ViewMode::COLOR_REG:
                    m_sdk.getColorReg(img);
                    break;
                default:
                    m_sdk.getColorDepth(img);
                    break;
            }

            imageView->setImg(img);
            imageView->show();
            m_sdk.m_newDataReady = false;
        }
        m_sdk.unlock();
    }
}


void MainWindow::about()
{
   QMessageBox::about(this, tr("Comet studio"),
            tr("The <b>Comet Studio</b> is an internal dev app "
               "used to test Comet SDK."));
}

void MainWindow::createActions()
{
    playAct    = new QAction(QIcon(":/icons/play.png"), tr("&Play"), this);
    connect(playAct, SIGNAL(triggered()), this, SLOT(play()));

    pauseAct   = new QAction(QIcon(":/icons/pause.png"), tr("&Pause"), this);
    connect(pauseAct, SIGNAL(triggered()), this, SLOT(pause()));

    stopAct    = new QAction(QIcon(":/icons/stop.png"), tr("&Stop"), this);
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stop()));

    recordAct  = new QAction(QIcon(":/icons/record.png"), tr("&Record"), this);
    connect(recordAct, SIGNAL(triggered()), this, SLOT(record()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(playAct);
    fileToolBar->addAction(pauseAct);
    fileToolBar->addAction(stopAct);
    fileToolBar->addAction(recordAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Logs"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    logsList = new QListWidget(dock);
    logsList->addItems(QStringList() << "TODO: Here go logs");
    dock->setWidget(logsList);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Create directory tree
    QDir* rootDir = new QDir(QDir::current());
    rootDir->cdUp();
    rootDir->cdUp();
    rootDir->cdUp();
    dirView = new DirWidget();
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(0,QString("File name"));
    dirView->setHeaderItem(headerItem);
    DirWidget::addTreeItems(dirView, nullptr, rootDir->absolutePath(), true);

    dock = new QDockWidget(tr("Dir"), this);
    dock->setWidget(dirView);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Set image view
    imageView = new ImageWidget();
    imageView->setBackgroundRole(QPalette::Base);
    imageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageView);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_1:
            m_viewMode = ViewMode::DEPTH;
            break;
        case Qt::Key_2:
            m_viewMode = ViewMode::IR;
            break;
        case Qt::Key_3:
            m_viewMode = ViewMode::COLOR;
            break;
        case Qt::Key_4:
            m_viewMode = ViewMode::COLOR_REG;
            break;
        default:
            break;
    }
}
