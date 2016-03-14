#include "qgis_dev_browsertreefilterproxymodel.h"

qgis_dev_browserTreeFilterProxyModel::qgis_dev_browserTreeFilterProxyModel( QObject *parent )
    : QSortFilterProxyModel( parent ),
      m_model( 0 ),
      m_filter( "" ),
      m_patternSyntax( "normal" ),
      m_caseSensitivity( Qt::CaseInsensitive )
{
    setDynamicSortFilter( true );
}

qgis_dev_browserTreeFilterProxyModel::~qgis_dev_browserTreeFilterProxyModel()
{

}

void qgis_dev_browserTreeFilterProxyModel::setBrowserModel( QgsBrowserModel* model )
{
    m_model = model;
    setSourceModel( model );
}

void qgis_dev_browserTreeFilterProxyModel::setFilterSyntax( const QString& syntax )
{
    if ( m_patternSyntax == syntax ) {return;}
    m_patternSyntax = syntax;
    updateFilter();
}

void qgis_dev_browserTreeFilterProxyModel::setFilter( const QString& filter )
{
    if ( m_filter == filter ) {return;}
    m_filter = filter;
    updateFilter();
}

void qgis_dev_browserTreeFilterProxyModel::setCaseSensitive( bool caseSensitive )
{
    m_caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    updateFilter();
}

void qgis_dev_browserTreeFilterProxyModel::updateFilter()
{
    m_reList.clear();
    if ( m_patternSyntax == "normal" )
    {
        foreach( QString f, m_filter.split( "|" ) )
        {
            QRegExp rx( QString( "*%1" ).arg( f.trimmed() ) );
            rx.setPatternSyntax( QRegExp::Wildcard );
            rx.setCaseSensitivity( m_caseSensitivity );
            m_reList.append( rx );
        }
    }
    else if ( m_patternSyntax == "wildcard" )
    {
        foreach ( QString f, m_filter.split( "|" ) )
        {
            QRegExp rx( f.trimmed() );
            rx.setPatternSyntax( QRegExp::Wildcard );
            rx.setCaseSensitivity( m_caseSensitivity );
            m_reList.append( rx );
        }
    }
    else
    {
        QRegExp rx( m_filter.trimmed() );
        rx.setPatternSyntax( QRegExp::RegExp );
        rx.setCaseSensitivity( m_caseSensitivity );
        m_reList.append( rx );
    }
    invalidateFilter();
}

bool qgis_dev_browserTreeFilterProxyModel::filterAcceptsString( const QString& value ) const
{
    if ( m_patternSyntax == "normal" || m_patternSyntax == "wildcard" )
    {
        foreach( QRegExp rx, m_reList )
        {
            if ( rx.exactMatch( value ) ) {return true;}
        }
    }
    else
    {
        foreach( QRegExp rx, m_reList )
        {
            if ( rx.indexIn( value ) != -1 ) {return true;}
        }
    }
    return false;
}

bool qgis_dev_browserTreeFilterProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
    if ( m_filter == "" || !m_model ) { return true; }

    QModelIndex sourceIndex = m_model->index( sourceRow, 0, sourceParent );
    return filterAcceptsItem( sourceIndex ) || filterAcceptsAncestor( sourceIndex ) || filterAcceptsDescendant( sourceIndex );
}

bool qgis_dev_browserTreeFilterProxyModel::filterAcceptsAncestor( const QModelIndex &sourceIndex ) const
{
    if ( !m_model )
    {
        return true;
    }

    QModelIndex sourceParentIndex = m_model->parent( sourceIndex );
    if ( !sourceParentIndex.isValid() )
    {
        return false;
    }
    if ( filterAcceptsItem( sourceParentIndex ) )
    {
        return true;
    }

    return filterAcceptsAncestor( sourceParentIndex );
}

bool qgis_dev_browserTreeFilterProxyModel::filterAcceptsDescendant( const QModelIndex &sourceIndex ) const
{
    if ( !m_model )
    {
        return true;
    }

    for ( int i = 0; i < m_model->rowCount( sourceIndex ); i++ )
    {
        QModelIndex sourceChildIndex = m_model->index( i, 0, sourceIndex );
        if ( filterAcceptsItem( sourceChildIndex ) )
        {
            return true;
        }
        if ( filterAcceptsDescendant( sourceChildIndex ) )
        {
            return true;
        }
    }
    return false;
}

bool qgis_dev_browserTreeFilterProxyModel::filterAcceptsItem( const QModelIndex &sourceIndex ) const
{
    if ( !m_model )
    {
        return true;
    }
    return filterAcceptsString( m_model->data( sourceIndex, Qt::DisplayRole ).toString() );
}


