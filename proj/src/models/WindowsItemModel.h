#ifndef WINDOWSITEMMODEL_H
#define WINDOWSITEMMODEL_H
#include <QAbstractItemModel>
#include <windows.h>

class WindowsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	enum MODE
	{
		MODE_CHILDS,
		MODE_SIBLINGS,
	};
private:
	MODE			mode;
	QList<HWND>		list;
public:
public:
	WindowsItemModel(QObject *parent, MODE mode);
	~WindowsItemModel();
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
	void*	getHandle(int i);
	void	setHandle(void* p);
};

#endif // WINDOWSITEMMODEL_H
