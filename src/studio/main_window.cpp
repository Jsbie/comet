#include <QtWidgets>

#include "main_window.h"

#include <iostream>

// SDK includes
#include "frame_pack.h"
#include "camera_types.h"
#include "input_data.h"

#include <QtWidgets>

MainWindow::MainWindow(QApplication* thisApp) :
    app(thisApp),
    m_isRunning(false)
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Comet Studio"));

    //newLetter();
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow() {
    stop();
}

void MainWindow::play() {
    stop();
    const QString path;
    m_sdk.initialize(CAMERA_FILE, path.toStdString().c_str());
    m_sdk.start();
    m_isRunning = true;
    m_sdkThread = std::thread(&MainWindow::run, this);
}

void MainWindow::pause() {
    std::cout << "pause" << std::endl;
}

void MainWindow::stop() {
    std::cout << "stop" << std::endl;
    m_isRunning = false;
    if (m_sdkThread.joinable()) {
        m_sdkThread.join();
    }
    m_sdk.stop();
}

void MainWindow::close() {
    QTimer::singleShot(250, app, SLOT(quit()));
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("Comet studio"),
            tr("The <b>Comet Studio</b> is an internal dev app "
               "used to test Comet SDK."));
}

void MainWindow::run() {
    while (!m_sdk.m_newDataReady){
        std::chrono::milliseconds time(1000);
        std::this_thread::sleep_for(time);
    }

    Image& img = m_sdk.m_data->m_input->color;

    QImage qimg = QImage(img.data, img.cols, img.rows, img.cols * 3, QImage::Format_RGB888);
    imageView->setPixmap(QPixmap::fromImage(qimg));
    imageView->show();
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
    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
            << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
            << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
            << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
            << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
            << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Paragraphs"), this);
    paragraphsList = new QListWidget(dock);
    paragraphsList->addItems(QStringList()
            << "Thank you for your payment which we have received today."
            << "Your order has been dispatched and should be with you "
               "within 28 days."
            << "We have dispatched those items that were in stock. The "
               "rest of your order will be dispatched once all the "
               "remaining items have arrived at our warehouse. No "
               "additional shipping charges will be made."
            << "You made a small overpayment (less than $5) which we "
               "will keep on account for you, or return at your request."
            << "You made a small underpayment (less than $1), but we have "
               "sent your order anyway. We'll add this underpayment to "
               "your next bill."
            << "Unfortunately you did not send enough money. Please remit "
               "an additional $. Your order will be dispatched as soon as "
               "the complete amount has been received."
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    dock->setWidget(paragraphsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Create directory tree
    QDir* rootDir = new QDir(QDir::current());
    rootDir->cdUp();
    rootDir->cdUp();
    rootDir->cdUp();
    tree = new DirWidget();
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(0,QString("File name"));
    tree->setHeaderItem(headerItem);
    DirWidget::addTreeItems(tree, nullptr, rootDir->absolutePath(), true);

    dock = new QDockWidget(tr("Dir"), this);
    dock->setWidget(tree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Set image view
    imageView = new QLabel();
    imageView->setBackgroundRole(QPalette::Base);
    imageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageView);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
}
