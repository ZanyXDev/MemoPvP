#include "appconfig.h"

bool AppConfig::isMobile() const { return m_isMobile; }

QString AppConfig::appVersion() const { return m_appVersion;}

QString AppConfig::buildQtVersion() const { return m_buildQtVersion; }

bool AppConfig::isDebugMode() const { return m_isDebugMode; }
