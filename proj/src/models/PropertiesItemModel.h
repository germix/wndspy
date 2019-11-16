#ifndef PROPERTIESITEMMODEL_H
#define PROPERTIESITEMMODEL_H
#include <QAbstractItemModel>
#include <windows.h>

class PropertiesItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	struct DATA
	{
		void*		handle;
		QString		text;
	};
private:
	HWND			hwnd;
	QList<DATA>		list;
public:
	PropertiesItemModel(QObject *parent);
	~PropertiesItemModel();
public:
	virtual QModelIndex index(int row, int column,
							  const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
signals:

public slots:
public:
	void	setHandle(void* p);
};

#endif // PROPERTIESITEMMODEL_H
