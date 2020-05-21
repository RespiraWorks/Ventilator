/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSETRANSITIONS_H
#define GREENHOUSETRANSITIONS_H

#include "greenhouse_global.h"

#include <QSignalTransition>
#include <QEvent>
#include <QVector>
#include <QVariantList>

#include <functional>

namespace GreenHouse {
class Context;
class State;
class ItemState;

///
/// \brief The TransitionFunctionAPI class defines an interface that lets us add test and trigger
/// functions to
/// transitions
///
class GREENHOUSESHARED_EXPORT TransitionFunctionAPI
{
    Q_DISABLE_COPY(TransitionFunctionAPI)
public:
    TransitionFunctionAPI() = default;
    ~TransitionFunctionAPI() = default;
    TransitionFunctionAPI(TransitionFunctionAPI &&) = delete;
    TransitionFunctionAPI &operator=(TransitionFunctionAPI &&) = delete;

    using TransitionTestFunc = std::function<bool(Context *, const QVariantList &)>;
    using TransitionTriggeredFunc = std::function<void(Context *, const QVariantList &)>;

    ///
    /// \brief addTestFunction used to add a new test function to a transition instance
    /// \param func
    ///
    /// Test functions are used to determine if a transition should trigger based on the execution
    /// context
    /// and the provided aregument list extracted from an event
    ///
    void addTestFunction(const TransitionTestFunc &func);

    ///
    /// \brief addTriggeredFunction used to add a new trigger function to a transition istance
    /// \param func
    ///
    /// Trigger functions are executed when a transition is triggered and get access to the
    /// execution
    /// context as well as the argument list extracted from the triggering event
    ///
    void addTriggeredFunction(const TransitionTriggeredFunc &func);

protected:
    bool runTests(Context *context, const QVariantList &args);
    void runTriggers(Context *context, const QVariantList &args);

private:
    QVector<TransitionTestFunc> m_testFunctions;
    QVector<TransitionTriggeredFunc> m_triggeredFunctions;
};

///
/// \brief The SignalTransition class a custom QSignalTransition subclass designed to work with a
/// Context instance
///
class GREENHOUSESHARED_EXPORT SignalTransition : public QSignalTransition,
                                                 public TransitionFunctionAPI
{
    Q_OBJECT
    Q_DISABLE_COPY(SignalTransition)
protected:
    explicit SignalTransition(State *sourceState);

public:
    ///
    /// \brief SignalTransition creates a new SignalTransition with the given sender, signal and
    /// source state
    /// \param sender a pointer to a sender object
    /// \param signal the name of the triggering signal
    /// \param sourceState the transition's source state
    ///
    explicit SignalTransition(const QObject *sender, const char *signal, State *sourceState);
    explicit SignalTransition(const QObject *sender, const QString &signalName, State *sourceState);
    ~SignalTransition() override = default;
    SignalTransition(SignalTransition &&) = delete;
    SignalTransition &operator=(SignalTransition &&) = delete;

    ///
    /// \brief transitionContext used to get the transition's execution context
    /// \return
    ///
    Context *transitionContext() const;

    ///
    /// \brief setTargetState used to set the transition's target using a unique state name
    /// \param stateName the target state's name
    ///
    void setTargetState(const QString &stateName);

    ///
    /// \brief setTargetState used to set the transition's target state
    /// \param state new target state
    ///
    void setTargetState(State *state);

    ///
    /// \brief parallelTriggeringAllowed used to get the transition's parallel triggering setting
    /// \return
    ///
    bool parallelTriggeringAllowed() const;

    ///
    /// \brief setParallelTriggeringAllowed used to set the transition's parallel triggering setting
    /// \param arg
    ///
    /// The parallel triggering setting is used to determine if this transition is allowed to
    /// trigger if
    /// the triggering event has already been handled by another transition parented by a parallel
    /// state
    /// (default true)
    ///
    void setParallelTriggeringAllowed(bool arg);

    ///
    /// \brief setBlockedIfTargetActive used to set the transition's block-if-target-active setting
    /// \param arg
    ///
    /// The block if target active setting can be used to block a transition from triggering if its
    /// target is already active.
    /// (default false)
    ///
    void setBlockedIfTargetActive(bool arg);

    ///
    /// \brief blockedIfTargetActive used to get the transition's blocked it target active setting
    /// \return
    ///
    bool blockedIfTargetActive() const;

    ///
    /// \brief setBackTransition used to set if this transition is a back trasition
    /// \param arg
    ///
    /// A back transition always determines its target based on it's sources back step chain
    ///
    void setBackTransition(bool arg);

    ///
    /// \brief backTransition check if the transition is a back transition
    /// \return
    ///
    bool backTransition() const;

    ///
    /// \brief stateMachineLevel used to get this transition's state machine level
    /// \return int
    ///
    /// The state machine level value determines the depth of this transition's source state inside
    /// the state machine
    ///
    int stateMachineLevel() const;

    ///
    /// \brief rawEventTest used to execute this transition's event tests
    /// \param event
    /// \return true if tests pass, false otherwise
    ///
    bool rawEventTest(QEvent *event);

    // QAbstractTransition interface
protected:
    bool eventTest(QEvent *event) override;

private:
    int m_stateMachineLevel;
    bool m_parallelTriggeringAllowed;
    bool m_blockedIfTargetActive;
    bool m_backTransition;
    QVariantList m_triggerArguments;
};

///
/// \brief The ItemPropertyChangedTransition class is a SignalTransition subclass designed to
/// trigger on UI element
/// interface property changes
///
class GREENHOUSESHARED_EXPORT ItemPropertyChangedTransition : public SignalTransition
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemPropertyChangedTransition)
public:
    enum CompareOperator {
        AnyChange = 0,
        Equal,
        NotEqual,
        GreaterThan,
        GreaterThanOrEqual,
        LessThan,
        LessThanOrEqual
    };

    ///
    /// \brief ItemPropertyChangedTransition creates a new ItemPropertyChangedTransition instance
    /// with the provided
    /// property, compare operator, trigger value and source state
    /// \param property the triggering property's name
    /// \param compareOperator the compare operator to be used when determineing if this transition
    /// should trigger
    /// \param triggerValue the value to be used when determineing if this transition should trigger
    /// \param sourceState the transition's source state
    ///
    explicit ItemPropertyChangedTransition(QString property, CompareOperator compareOperator,
                                           QVariant triggerValue, ItemState *sourceState);
    ~ItemPropertyChangedTransition() override = default;
    ItemPropertyChangedTransition(ItemPropertyChangedTransition &&) = delete;
    ItemPropertyChangedTransition &operator=(ItemPropertyChangedTransition &&) = delete;

private:
    const QString m_property;
    const QVariant m_triggerValue;
    const CompareOperator m_compareOperator;
};

///
/// \brief The ItemSignalTransition class is a SignalTransition subclass designed to trigger on UI
/// element interface
/// signals
///
class GREENHOUSESHARED_EXPORT ItemSignalTransition : public SignalTransition
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemSignalTransition)
public:
    ///
    /// \brief ItemSignalTransition creates a new ItemSignalTransition instance with the provided
    /// signal name and source
    /// state
    /// \param signalName the triggering signal's name
    /// \param sourceState the transition's source state
    ///
    explicit ItemSignalTransition(QString signalName, ItemState *sourceState);
    ~ItemSignalTransition() override = default;
    ItemSignalTransition(ItemSignalTransition &&) = delete;
    ItemSignalTransition &operator=(ItemSignalTransition &&) = delete;

private:
    const QString m_signalName;
    ItemState *m_sourceState;
};
}

#endif // GREENHOUSETRANSITIONS_H
