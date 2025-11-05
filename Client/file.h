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
    QString m_strUserPath;//因为files不在Client目录下，所以要存是谁的文件，初始位置是什么
    QString m_strCurPath;
    QList<FileInfo*> m_pFileInfoList;
    QString m_strMoveFileName;
    QString m_strMoveFilePath;
    QString m_strUploadFilePath;//上传的文件路径

    void flushFile();
    void updateFile_LW(QList<FileInfo*> pFileList);
    void uploadFile();

public slots:
    void errorSlot(QStirng error);

private slots:
    void on_mkdir_Pb_clicked();

    void on_flush_Pb_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_Pb_clicked();


    void on_remove_Pb_clicked();

    void on_upload_Pb_clicked();


private:
    Ui::File *ui;
};

#endif // FILE_H
