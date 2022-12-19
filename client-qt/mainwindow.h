#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class HTMLDelegate;
class QLineEdit;
class QNetworkAccessManager;
class QNetworkReply;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSearch();
    void showResults(QNetworkReply *reply);
    void openItem(const QModelIndex &index);

private:
    QNetworkAccessManager *qnam;

    QLineEdit *search;
    QTableWidget *resultsTable;
    HTMLDelegate *htmlDelegate;
};
#endif // MAINWINDOW_H
