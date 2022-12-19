#include "htmldelegate.h"
#include "mainwindow.h"

#include <QDesktopServices>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTableWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("full-text-search client");

    qnam = new QNetworkAccessManager(this);

    QWidget *window = new QWidget;
    setCentralWidget(window);

    QVBoxLayout *l = new QVBoxLayout(window);

    search = new QLineEdit;
    l->addWidget(search);

    resultsTable = new QTableWidget(0,3);
    htmlDelegate = new HTMLDelegate();
    resultsTable->setItemDelegate(htmlDelegate);
    resultsTable->setHorizontalHeaderLabels({"URL", "Title", "Body"});
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->horizontalHeader()->setResizeContentsPrecision(20);
    l->addWidget(resultsTable);

    connect(search, &QLineEdit::returnPressed, this, &MainWindow::runSearch);
    connect(qnam, &QNetworkAccessManager::finished, this, &MainWindow::showResults);
    connect(resultsTable, &QTableWidget::doubleClicked, this, &MainWindow::openItem);
}

MainWindow::~MainWindow()
{
}

void MainWindow::runSearch()
{
    QUrl req = QUrl("http://127.0.0.1:38589/search?q=" + QUrl::toPercentEncoding(search->text()));
    qnam->get(QNetworkRequest(req));
}

void MainWindow::showResults(QNetworkReply *reply)
{
    QByteArray body = reply->readAll();
    reply->deleteLater();

    resultsTable->clearContents();

    QJsonArray results = QJsonDocument::fromJson(body).array();
    resultsTable->setRowCount(results.count());
    int row = 0;
    for (const QJsonValue &result : results)
    {
        QJsonObject resultObj = result.toObject();
        QString url = resultObj.find("url")->toString();
        QString title = resultObj.find("title")->toString();
        QString body = resultObj.find("result")->toString();

        resultsTable->setItem(row, 0, new QTableWidgetItem(url));
        resultsTable->setItem(row, 1, new QTableWidgetItem(title));

        QTableWidgetItem *bodyItem = new QTableWidgetItem();
        bodyItem->setData(Qt::DisplayRole, QVariant::fromValue(HTMLDelegate::HTMLText { body }));
        resultsTable->setItem(row, 2, bodyItem);
        ++row;
    }

    resultsTable->resizeColumnToContents(1);
    resultsTable->resizeColumnToContents(2);

    // resize window to contents
    QHeaderView *hh = resultsTable->horizontalHeader();
    int width = 0;
    for (int i=0; i<3; ++i)
        width += hh->sectionSize(i);
    this->resize(QSize(width, this->height()));
}

void MainWindow::openItem(const QModelIndex &index)
{
    if (index.column() != 0) return;

    QString url = index.data().toString();
    QDesktopServices::openUrl(QUrl(url));
}
