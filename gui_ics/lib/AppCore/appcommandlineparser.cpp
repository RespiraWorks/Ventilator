#include "appcommandlineparser.h"

#include <QCoreApplication>
#include <QCommandLineParser>

namespace {

QString parseDebugOptions(const QString &debug)
{
    if (debug.contains(QStringLiteral("all")))
        return QStringLiteral("*.debug=true");
    else if (debug.contains(QStringLiteral("none")))
        return QStringLiteral("*.debug=false");
    QString res = QStringLiteral("*.debug=false\n");
    const QStringList enabledOptions = debug.split(QLatin1Char(','), QString::SkipEmptyParts);
    for (const QString &option : enabledOptions)
        res.append(QStringLiteral("%1.debug=true\n").arg(option.trimmed()));
    return res;
}
}

AppCommandLineParser::AppCommandLineParser(const QStringList &args)
  : m_loggingFilters(QStringLiteral("*.debug=false\n"
                                    "qml.debug=true\n"
                                    "theming.debug=true\n"
                                    "greenhouse.debug=true\n"
                                    "appcore.debug=true"))
  , m_remoteBackend(false)
  , m_backendPort(26185)
  , m_backendUrl(QStringLiteral("localhost"))
  , m_simulator(false)
  , m_simulatorPort(26186)
  , m_simulatorUrl(QStringLiteral("localhost"))
{
    QCommandLineParser parser;
    parser.setApplicationDescription(qApp->applicationName());
    const QCommandLineOption versionOption = parser.addVersionOption();
    const QCommandLineOption helpOption = parser.addHelpOption();

    // Add additional debug options here
    QLatin1String debugOptionDescription =
            QLatin1String("Enable debug options:\n"
                          "with <component> being a comma separated list of:\n"
                          "      greenhouse: enable GreenHouse library log messages\n"
                          "         theming: enable GreenHouse theming log messages\n"
                          "             rpc: enable GreenHouse RPC log messages\n"
                          "    statemachine: enable GreenHouse state machine log messages\n"
                          "         appcore: enable AppCore library log messages\n"
                          "             qml: enable QML console log messages\n"
                          "             all: Turn all log messages on (noisy)\n"
                          "            none: Turn all log messages off (default)\n");
    const QCommandLineOption debugOption(
            { QStringLiteral("d"), QStringLiteral("debug") },
            QCoreApplication::translate("main", debugOptionDescription.latin1()),
            QStringLiteral("component"), QStringLiteral("none"));
    parser.addOption(debugOption);

    QCommandLineOption pluginDir(
            { QStringLiteral("c"), QStringLiteral("pluginDir") },
            QCoreApplication::translate("main",
                                        "Used to specify a path to the plugin directory.\n"
                                        "   e.g. '/usr/lib64/plugin-dir' \n"),
            QStringLiteral("path"), QString());
    parser.addOption(pluginDir);

    QCommandLineOption remoteBackend(
            { QStringLiteral("remoteBackend") },
            QCoreApplication::translate(
                    "main",
                    "Used to sepcify if the application should connect to a remote backend.\n"));
    parser.addOption(remoteBackend);

    QCommandLineOption backendPort(
            { QStringLiteral("backendPort") },
            QCoreApplication::translate(
                    "main",
                    "Used to sepcify the port used to connect to a backend instance.\n"
                    "   e.g. '26186'. \n"),
            QStringLiteral("port"));
    parser.addOption(backendPort);

    QCommandLineOption backendUrl(
            { QStringLiteral("backendUrl") },
            QCoreApplication::translate(
                    "main",
                    "Used to sepcify the URL used to connect to a backend instance.\n"
                    "   e.g. 'localhost'. \n"),
            QStringLiteral("url"), m_backendUrl);
    parser.addOption(backendUrl);

    QCommandLineOption simulator(
            { QStringLiteral("simulator") },
            QCoreApplication::translate(
                    "main", "Used to sepcify if the application should connect to a simulator.\n"));
    parser.addOption(simulator);

    QCommandLineOption simulatorPort(
            { QStringLiteral("simulatorPort") },
            QCoreApplication::translate(
                    "main",
                    "Used to sepcify the port used to connect to a simulator instance.\n"
                    "   e.g. '26186'. \n"),
            QStringLiteral("port"));
    parser.addOption(simulatorPort);

    QCommandLineOption simulatorUrl(
            { QStringLiteral("simulatorUrl") },
            QCoreApplication::translate(
                    "main",
                    "Used to sepcify the URL used to connect to a simulator instance.\n"
                    "   e.g. 'localhost'. \n"),
            QStringLiteral("url"), m_simulatorUrl);
    parser.addOption(simulatorUrl);

    parser.process(args);

    if (parser.isSet(versionOption))
        parser.showVersion();

    if (parser.isSet(helpOption))
        parser.showHelp();

    if (parser.isSet(debugOption))
        m_loggingFilters = parseDebugOptions(parser.value(debugOption));

    if (parser.isSet(pluginDir))
        m_pluginPath = parser.value(pluginDir);

    m_remoteBackend = parser.isSet(remoteBackend);

    if (parser.isSet(backendPort))
        m_backendPort = parser.value(backendPort).toInt();

    if (parser.isSet(backendUrl))
        m_backendUrl = parser.value(backendUrl);

    m_simulator = parser.isSet(simulator);

    if (parser.isSet(simulatorPort))
        m_simulatorPort = parser.value(simulatorPort).toInt();

    if (parser.isSet(simulatorUrl))
        m_simulatorUrl = parser.value(simulatorUrl);
}

QString AppCommandLineParser::loggingFilters() const
{
    return m_loggingFilters;
}

QString AppCommandLineParser::pluginPath() const
{
    return m_pluginPath;
}

bool AppCommandLineParser::remoteBackend() const
{
    return m_remoteBackend;
}

int AppCommandLineParser::backendPort() const
{
    return m_backendPort;
}

QString AppCommandLineParser::backendUrl() const
{
    return m_backendUrl;
}

bool AppCommandLineParser::simulator() const
{
    return m_simulator;
}

int AppCommandLineParser::simulatorPort() const
{
    return m_simulatorPort;
}

QString AppCommandLineParser::simulatorUrl() const
{
    return m_simulatorUrl;
}
