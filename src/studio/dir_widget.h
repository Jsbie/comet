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

    /// Adds items to a parent node.
    static void addTreeItems(QTreeWidget* widget, QTreeWidgetItem* parent, QString filePath, bool isTopLevel = false) {
        QDir* rootDir = new QDir(filePath);
        QFileInfoList filesList = rootDir->entryInfoList();

        foreach(QFileInfo fileInfo, filesList)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0,fileInfo.fileName());

            if(fileInfo.isFile())
            {
              item->setIcon(0,*(new QIcon(":/icons/file.png")));
            }
            if(fileInfo.isDir())
            {
              item->setIcon(0,*(new QIcon(":/icons/folder.png")));
              if (isTopLevel) {
                  addTreeItems(nullptr, item, fileInfo.filePath(), false);
              }
            }

            if (isTopLevel) {
                widget->addTopLevelItem(item);
            } else {
                parent->addChild(item);
            }
        }
    }

public slots:
  /// Shows another directory level
  void showDirectory(QTreeWidgetItem* item, int /*column*/)
  {
    addTreeItems(nullptr, item, item->text(1), false);
    resizeColumnToContents(0);
  }

};

#endif // DIR_WIDGET_H
