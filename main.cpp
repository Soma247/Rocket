#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <iostream>
#include <memory>
#include "ballisticcalc.h"
#include "Gui/workwindow.h"
#include "algorithm"
constexpr int LOAD_TIME_MSEC = 1000;
constexpr int PROGRESS_X_PX = 272;
constexpr int PROGRESS_Y_PX = 333;
constexpr int PROGRESS_WIDTH_PX = 310;
constexpr int PROGRESS_HEIGHT_PX = 28;

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QPixmap pix( "D:/Qt/projects/Rocket/resources/splashscreen.png" );
    QSplashScreen splashScreen( pix );
    splashScreen.show();
    app.processEvents();
    QTime time;
    time.start();
    while(time.elapsed()<LOAD_TIME_MSEC) {
        int progress = static_cast<int>(100.0*time.elapsed()/LOAD_TIME_MSEC);
        splashScreen.showMessage(
                    QObject::trUtf8( "Загружено: %1%" ).arg(progress),
                    Qt::AlignBottom | Qt::AlignRight
                    );
        QPainter painter;
        painter.begin( &pix );
        painter.fillRect(
                    PROGRESS_X_PX,
                    PROGRESS_Y_PX,
                    static_cast<int>(progress*PROGRESS_WIDTH_PX/100.0 ),
                    PROGRESS_HEIGHT_PX, Qt::red
                    );

        painter.end();
        splashScreen.setPixmap( pix );
        app.processEvents();
    }
    splashScreen.close();

    WorkWindow ww("D:/Qt/projects/Rocket/materials.txt","D:/Qt/projects/Rocket/hardfuels.txt","D:/Qt/projects/Rocket/projects.ini");
    return app.exec();
}
