#include "NavView.h"
#include <QtWidgets/QApplication>
#include <QFile>

static void initStyleSheet(QApplication& a, QString file)
{
	//Q_INIT_RESOURCE(ads); // If static linked.
	QFile f(file);
	if (f.open(QFile::ReadOnly))
	{
		const QByteArray ba = f.readAll();
		f.close();
		a.setStyleSheet(QString(ba));
	}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//initStyleSheet(a, ":/res/style.css");

    NavView w;
    w.show();
    return a.exec();
}
