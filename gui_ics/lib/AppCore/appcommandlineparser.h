#ifndef APPCOMMANDLINEPARSER_H
#define APPCOMMANDLINEPARSER_H

#include "appcore_global.h"
#include <QString>

///
/// \brief The AppCommandLineParser class is used to parse the command line arguments passed to the
/// application
///
class APPCORE_EXPORT AppCommandLineParser
{
public:
    ///
    /// \brief AppCommandLineParser AppCommandLineParser creates a new AppCommandLineParser instance
    /// \param args - a list of command-line arguments
    ///
    AppCommandLineParser(const QStringList &args);

    ///
    /// \brief loggingFilters used to get a string containing the logging category filters
    /// \return
    ///
    QString loggingFilters() const;

    ///
    /// \brief pluginPath used to get a string containing the plugin loading path
    /// \return
    ///
    QString pluginPath() const;

    ///
    /// \brief remoteBackend used to determine if the application should connect to a remote
    ///  backend instead of loading local plugins
    ///
    /// \return
    ///
    bool remoteBackend() const;

    ///
    /// \brief backendPort used to get a string containing the port used to connect to a remote
    /// backend
    ///
    /// \return
    ///
    int backendPort() const;

    ///
    /// \brief backendUrl used to get a string containing the url used to connect to a remote
    /// bakcend
    ///
    /// \return
    ///
    QString backendUrl() const;

    ///
    /// \brief simulator used to determine if the application should connect to a simulator
    /// \return
    ///
    bool simulator() const;

    ///
    /// \brief simulatorPort used to get a string containing the port used to connect to a simulator
    /// \return
    ///
    int simulatorPort() const;

    ///
    /// \brief simulatorUrl used to get a string containing the url used to connect to a simulator
    /// instance \return
    ///
    QString simulatorUrl() const;

private:
    QString m_loggingFilters;
    QString m_pluginPath;

    bool m_remoteBackend;
    int m_backendPort;
    QString m_backendUrl;

    bool m_simulator;
    int m_simulatorPort;
    QString m_simulatorUrl;
};

#endif // APPCOMMANDLINEPARSER_H
