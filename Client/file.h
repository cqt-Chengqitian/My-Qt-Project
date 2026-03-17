#ifndef FILE_H
#define FILE_H

#include "protocol.h"

#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = 0);
    ~File();
    QString m_strUserPath; // 存储用户的文件路径
    QString m_strCurPath; // 当前路径
    QList<FileInfo*> m_pFileInfoList; // 文件列表
    QString m_strMoveFileName; // 移动文件的名
    QString m_strMoveFilePath; // 移动文件的路径
    QString m_strUploadFilePath; // 上传的文件路径

    void flushFile(); // 刷新文件列表
    void updateFile_LW(QList<FileInfo*> pFileList); // 上传文件
    void uploadFile();

public slots:
    void errorSlot(QString error);

private slots:
    void on_mkdir_Pb_clicked();

    void on_flush_Pb_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item); // 双击选中展示窗口的文件

    void on_return_Pb_clicked();

    void on_remove_Pb_clicked();

    void on_upload_Pb_clicked();


private:
    Ui::File *ui;
};

#endif // FILE_H
