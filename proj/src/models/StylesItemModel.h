#ifndef STYLESITEMMODEL_H
#define STYLESITEMMODEL_H
#include <tchar.h>
#include <windows.h>

#include <QAbstractItemModel>


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//
//	Simple style-lookup
//
typedef struct
{
	UINT    style;
	LPCWSTR szName;

} StyleLookupType;

//
//	Extended Style table. 1 per window class
//
typedef struct
{
	DWORD   style;		// Single window style
	LPCWSTR name;		// Textual name of style

	DWORD   cmp_mask;	// If zero, then -style- is treated as a single bit-field
						// Otherwise, cmp_mask is used to make sure that
						// ALL the bits in the mask match -style-

	DWORD   depends;	// Style is only valid if includes these styles
	DWORD   excludes;	// Style is only valid if these aren't set

} StyleLookupEx;

//
//	Use this helper-macro to fill in the first two members
//  of the style structures.
//
//  e.g. STYLE_(WS_CHILD)  ->  WS_CHILD, "WS_CHILD"
//
#define STYLE_(style) (UINT)style, _T(#style)

//
//	Use this structure to list each window class with its
//	associated style table
//
typedef struct
{
	LPCWSTR			szClassName;
	StyleLookupEx*	stylelist;
	DWORD			dwGetData;
	DWORD			dwSetData;
} ClassStyleLookup;

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class StylesItemModel : public QAbstractItemModel
{
	Q_OBJECT
public:

	enum MODE
	{
		MODE_NORMAL,
		MODE_EXTENDED,
		MODE_CONTROL,
	};
	struct DATA
	{
		QString		name;
		DWORD		style;
	public:
		DATA()
		{
		}
		DATA(const QString& n, DWORD s) : name(n), style(s)
		{
		}
		~DATA()
		{
		}
	};
private:
	MODE				mode;
	HWND				hwnd;
	QList<DATA>			list;
	DWORD				style;

	bool				edition;
public:
public:
	StylesItemModel(QObject* parent, bool edition);
	~StylesItemModel();
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
	DWORD		getStyle() const;
	void		setStyle(HWND hwnd, MODE mode, BOOL fAllStyles, DWORD* pStyle);

};

#endif // STYLESITEMMODEL_H
