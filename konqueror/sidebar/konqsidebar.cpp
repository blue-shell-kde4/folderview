/***************************************************************************
                               konqsidebar.cpp
                             -------------------
    begin                : Sat June 2 16:25:27 CEST 2001
    copyright            : (C) 2001 Joseph Wenninger
    email                : jowenn@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "konqsidebar.h"
#include <kaboutdata.h>

#include <konq_events.h>
#include <kdebug.h>
#include <QtGui/QApplication>
#include <kacceleratormanager.h>

static KAboutData createAboutData()
{
    KAboutData aboutData("konqsidebartng", 0, ki18n("Extended Sidebar"), "0.2");
    aboutData.addAuthor(ki18n("Joseph Wenninger"), KLocalizedString(), "jowenn@bigfoot.com");
    aboutData.addAuthor(ki18n("David Faure"), KLocalizedString(), "faure@kde.org");
    return aboutData;
}

K_PLUGIN_FACTORY(KonqSidebarFactory,
                 registerPlugin<KonqSidebar>();
    )
K_EXPORT_COMPONENT_FACTORY(konq_sidebar, KonqSidebarFactory(createAboutData()))

KonqSidebar::KonqSidebar(QWidget *parentWidget, QObject *parent, const QVariantList&)
    : KParts::ReadOnlyPart(parent)
{
	// we need an instance
	setComponentData(KonqSidebarFactory::componentData());

	// this should be your custom internal widget
	m_widget = new Sidebar_Widget( parentWidget, this, parentWidget->window()->property("currentProfile").toString() );
	m_extension = new KonqSidebarBrowserExtension( this, m_widget );
	connect(m_widget,SIGNAL(started(KIO::Job *)),
		this, SIGNAL(started(KIO::Job*)));
	connect(m_widget,SIGNAL(completed()),this,SIGNAL(completed()));
	connect(m_extension, SIGNAL(addWebSideBar(const KUrl&, const QString&)),
		m_widget, SLOT(addWebSideBar(const KUrl&, const QString&)));
        KAcceleratorManager::setNoAccel(m_widget);
	setWidget(m_widget);
}

KonqSidebar::~KonqSidebar()
{
}

bool KonqSidebar::openFile()
{
	return true;
}

bool KonqSidebar::openUrl(const KUrl &url) {
    return m_widget->openUrl(url);
}

void KonqSidebar::customEvent(QEvent* ev)
{
	if (KonqFileSelectionEvent::test(ev) ||
	    KonqFileMouseOverEvent::test(ev) ||
	    KonqConfigEvent::test(ev))
	{
		// Forward the event to the widget
		QApplication::sendEvent( widget(), ev );
	}
}

#include "konqsidebar.moc"
