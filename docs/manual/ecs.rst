The Entity Component System
===========================

Events and Systems
------------------

The primary method to interact with the game world is through events and event callbacks. A *system* is loosely defined as a 
collection of event callbacks. For example, use the ``UpdateEvent`` to call some code each frame

.. code:: cpp

  void OnUpdateEvent(const UpdateEvent& event)
  {
    // code
  }

Lotus provides two methods to register these callbacks, depending on whether the callback is a free function or a class member.

.. code:: cpp

  eventManager.Bind<FoobarEvent, OnFoobarEvent>();
  eventManager.Bind<FoobarEvent, &FoobarSystemClass::OnFoobarEvent>(instanceOfFoobarSystemClass);

These callbacks will then be called whenever ``FoobarEvent`` is invoked. Note that the order in which these callbacks are executed 
is not guaranteed. To invoke an event, use ``EventManager::Dispatch()``, with an event object passed as a parameter. The ``Event::Immediate`` flag 
controls when the event is actually called. Immediate events suspend execution and call all registered callbacks. Non-immediate 
events are queued and handled at the end of each engine tick, after the ``PostUpdateEvent`` has been called.

Common Patterns
+++++++++++++++

Events and systems are extremely flexible and can be used however the user wants to use them. We follow the conventions outlined below 
while developing the engine.

Systems may or may not maintain state. For example the ``CameraSystem`` does not maintain state, while the ``Input`` system does. 
Stateless systems can be easily created with free functions in a namespace, and can therefore be registered without passing an 
object. Stateful systems on the other hand may further be of two types: those that persist for the entire duration of the game or 
those that have a defined lifetime. The latter is simple to deal with, by creating a class with relevant data and callbacks as 
class methods. An object of this class can be created whenever this system's lifetime starts. Core systems like physics and the 
engine tick itself are examples of the former kind. These systems are again defined as namespaces, but callbacks are now implemented 
necessarily in a separate ``system.cpp`` file where static state variables are defined.


Component Serialization
-----------------------




