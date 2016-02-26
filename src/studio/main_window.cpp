#include <QtWidgets>

#include "main_window.h"

#include <iostream>
#include "log.h"

// SDK includes
#include "frame_pack.h"
#include "camera_types.h"
#include "input_data.h"

#include <QtWidgets>

MainWindow::MainWindow(QApplication* thisApp) :
    app(thisApp),
    m_viewMode(ViewMode::DEPTH),
    m_isRunning(false)
{
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
    Log::d("play", "STUDIO");
    const QString path;
    bool initialized = m_sdk.initialize(CAMERA_KINECT2, path.toStdString().c_str());
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
    Log::d("stop", "STUDIO");

    m_isRunning = false;
    if (m_sdkThread.joinable()) {
        m_sdkThread.join();
    }
}

void MainWindow::close() {
    QTimer::singleShot(250, app, SLOT(quit()));
}

void MainWindow::setColorDepth(Image& img) {
    m_sdk.getColorDepth(img);
}

void MainWindow::setColorIr(Image& img) {
    m_sdk.getColorIr(img);
}

void MainWindow::setColor(Image& img) {
    Image& in = m_sdk.m_data->m_input->color;
    img.updateSize(in.rows, in.cols, in.bytesPerPixel);
    img.swapData(&in.data);
}

void MainWindow::run() {
    while (m_isRunning){
        std::chrono::milliseconds time(15);
        std::this_thread::sleep_for(time);

        m_sdk.lock();
        if (m_sdk.m_newDataReady) {
            Image tmp;
            switch (m_viewMode) {
                case ViewMode::DEPTH:
                    setColorDepth(tmp);
                    break;
                case ViewMode::IR:
                    setColorIr(tmp);
                    break;
                case ViewMode::COLOR:
                    setColor(tmp);
                    break;
                default:
                    setColorDepth(tmp);
                    break;
            }

            imageView->setImg(tmp);
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
        default:
            break;
    }
}
