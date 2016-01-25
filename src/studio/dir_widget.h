#ifndef DIR_WIDGET_H
#define DIR_WIDGET_H

#include <QtGui>
#include <QTreeWidget>

class DirWidget : public QTreeWidget
{
  Q_OBJECT
public:
    DirWidget()
    {
      // Add Subdirectories as children 	when user clicks on a file item,
      // otherwise adding all children recursively may consume HUGE amount of memory
      connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showDirectory(QTreeWidgetItem*, int)));
    }
    ~DirWidget(){}

public slots:
  void showDirectory(QTreeWidgetItem* item, int /*column*/)
  {
    QDir* rootDir = new QDir(item->text(1));
    QFileInfoList filesList = rootDir->entryInfoList();

    foreach(QFileInfo fileInfo, filesList)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setText(0,fileInfo.fileName());

        if(fileInfo.isFile())
        {
          child->setIcon(0,*(new QIcon(":/icons/file.png")));
        }
        if(fileInfo.isDir())
        {
          child->setIcon(0,*(new QIcon(":/icons/folder.png")));
        }

        item->addChild(child);

        resizeColumnToContents(0);
    }
  }

};

#endif // DIR_WIDGET_H
