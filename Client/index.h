#ifndef INDEX_H
#define INDEX_H

#include "file.h"
#include "friend.h"

#include <QWidget>

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    ~Index();
    static Index& getInstance();
    Friend* getFriend();
    File* getFile();

private slots:
    void on_friend_Pb_clicked();

    void on_pushButton_2_clicked();

private:
    explicit Index(QWidget *parent = 0);
    Index(const Index& other) = delete;
    Index& operator =(const Index) = delete;
    Ui::Index *ui;
};

#endif // INDEX_H
