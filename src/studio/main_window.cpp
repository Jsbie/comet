#include <QtWidgets>

#include "main_window.h"

MainWindow::MainWindow(QApplication* thisApp) :
    app(thisApp)
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

void MainWindow::play() {
    const QString path;
    m_sdk.initialize(0, path.toStdString().c_str());
}

void MainWindow::pause() {

}

void MainWindow::stop() {

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

}
