#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QSplitter>
#include "view.h"

/**
 * Handles the creation of the graphics view and scene, and
 * initialises the outer walls.
 */
class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

public slots:
    void createBox(qreal x, qreal y);
    void createWall(QLineF l, bool canBeDeleted = true);

private:
    QGraphicsScene* mScene;
    View* mView;
};

