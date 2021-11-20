#ifndef QDMTREEVIEWGRAPHS_H
#define QDMTREEVIEWGRAPHS_H

#include <zeno/common.h>
#include "qdmgraphicsscene.h"
#include <QTreeView>
#include <QStandardItem>
#include <memory>
#include <vector>

ZENO_NAMESPACE_BEGIN

class QDMTreeViewGraphs : public QTreeView
{
    Q_OBJECT

    QDMGraphicsScene *rootScene;

    std::vector<std::unique_ptr<QStandardItem>> raiiItems;

public:
    explicit QDMTreeViewGraphs(QWidget *parent = nullptr);
    ~QDMTreeViewGraphs();

    virtual QSize sizeHint() const override;

public slots:
    void setRootScene(QDMGraphicsScene *scene);
    void refreshRootScene();
    void switchScene(QDMGraphicsScene *scene);

signals:
    void rootSceneChanged(QDMGraphicsScene *scene);
    void sceneSwitched(QDMGraphicsScene *scene);
};

ZENO_NAMESPACE_END

#endif // QDMTREEVIEWGRAPHS_H