#include "namize.h"
#include "ui_namize.h"

#include <QNetworkInterface>
#include <QCryptographicHash>
#include <array>
#include <QFile>

Namize::Namize(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Namize)
{
    ui->setupUi(this);

    ui->resultat->hide();
}

Namize::~Namize()
{
    delete ui;
}

QString getMacAddress()
{
    // return QNetworkInterface::interfaceFromIndex(0).hardwareAddress();
    return QNetworkInterface::allInterfaces().first().hardwareAddress();
}

QString toMd5(QString chaine) {
    return QString(QCryptographicHash::hash(chaine.toUtf8(), QCryptographicHash::Md5).toHex());
}

int getMin(int nb1, int nb2) {
    return nb1 < nb2 ? nb1 : nb2;
}

int compareStrings(QString lhs, QString rhs) {
    int len0 = lhs.length() + 1;
    int len1 = rhs.length() + 1;

    std::vector<int> cost;
    cost.reserve(len0);
    std::vector<int> newcost;
    newcost.reserve(len0);

    for(int i = 0; i < len0; i++) {
        cost[i] = i;
    }

    for(int j = 1; j < len1; j++) {
        newcost[0] = j;

        for(int i = 1; i < len0; i++) {
            int match = (lhs.at(i - 1) == rhs.at(j - 1)) ? 0 : 1;

            int cost_replace = cost[i - 1] + match;
            int cost_insert = cost[i] + 1;
            int cost_delete = newcost[i - 1] + 1;

            newcost[i] = getMin(getMin(cost_insert, cost_delete), cost_replace);
        }
        cost.swap(newcost);
    }
    return cost[len0 - 1];
}

void Namize::on_pushButton_clicked()
{
    QString test = getMacAddress();
    QString testMd5 = toMd5(test);

    int distance = -1;
    QString plusProche = "Inconnu";
    bool fileSexe = false;
    bool sexe = false; // true pour une fille

    int newDistance;
    QFile inputFile(":masculin.txt");
    if (inputFile.open(QIODevice::ReadOnly)) {
       QTextStream in(&inputFile);
       while(!in.atEnd()) {
          QString line = in.readLine();
          newDistance = compareStrings(testMd5, toMd5(line));
          if(distance == -1 || distance >= newDistance) {
              distance = newDistance;
              plusProche = line;
              sexe = fileSexe;
          }
       }
       inputFile.close();
    }
    fileSexe = true;
    QFile inputFile2(":feminin.txt");
    if (inputFile2.open(QIODevice::ReadOnly)) {
       QTextStream in(&inputFile2);
       while(!in.atEnd()) {
          QString line = in.readLine();
          newDistance = compareStrings(testMd5, toMd5(line));
          if(distance == -1 || distance >= newDistance) {
              distance = newDistance;
              plusProche = line;
              sexe = fileSexe;
          }
       }
       inputFile2.close();
    }

    ui->pushButton->hide();
    ui->textNom->setText(plusProche.left(1).toUpper() + plusProche.mid(1));
    ui->textSexe->setText(sexe ? "Fille" : "Garçon");
    ui->resultat->show();
}
