Quickstart
=============

This page describes the provided quickstart example and some key concepts that one should be aware of when using the
engine. Head over to ``examples/quickstart`` (this will be assumed to be the root directory from now on). The suggested project
structure includes a ``main.cpp`` file, a ``CMakeLists.txt`` and a ``resources/`` directory. We'll focus on what's happening
in ``main.cpp`` first.

Configuration
--------------

Lotus is a library and depends on the user to supply a main function. The first step is to initialize the engine and specify
configuration options. This is done through the ``.lproject`` file, kept alongside your ``CMakeLists.txt``. ::

    LoadConfig(PROJECT_SETTINGS);
    Lotus::Initialize();

For now, we have to explicitly tell Lotus where this file is. A ``PROJECT_SETTINGS`` macro is defined that can be passed if
the file is in its default location.

The project file is processed by CMake before being used by the engine, and therefore variables from the build system can
be easily used. The ``Project.ProjectResourceRoot`` field exploits this to point to the ``resources/`` directory of the project.
This way one does not need to use absolute paths in the configuration file. To insert the value of a CMake variable ``<VAR>``,
simply surround it with '@' like so ::

    "KeyFilledByCMake": "@<VAR>@"

Scenes
------

Scene files describe entities that can be loaded together. The ``SceneManager`` can be used for loading and unloading scenes,
and that is what we're going to do next.::

    SceneManager::LoadScene(ExpandPath("res://scenes/SampleScene.json"));

The ``LoadScene`` function, like all other loaders in Lotus, take the absolute path of the resource as argument. The
``ExpandPath`` function resolves a relative path and returns its absolute counterpart.

There are two roots when dealing with resources: ``res://`` and ``int://``. The former refers to your project's resources,
and uses the path set with ``SetProjectResourceRoot``. ``int://`` on the other hand refers to the resources supplied with the
engine, like basic meshes and shaders. It resolves to whatever path the engine sources are on.

For more information on how entities are actually serialized in ``SampleScene.json``, refer to :ref:`Component Serialization`.

Events
------

The next few lines register custom events to control the camera. Lotus provides some lifecycle events (``UpdateEvent``)
and some input events (``MouseEvent``) that can be registered with the event manager. ``UpdateEvent`` is called every
render frame and therefore depends on the framerate. Use the ``UpdateEvent::DeltaTime`` field to get the time since the
last frame.::

    EventManager& em = GET(EventManager);
    em.Bind<UpdateEvent, CameraSystem::OnUpdate>();
    em.Bind<MouseEvent, CameraSystem::OnMouseEvent>();

``GET(X)`` is shorthand for ``X::Get()``, which fetches the singleton ``X``.

Main loop
---------

Once all the configuration is done the main engine loop can be started with ``Lotus::Run()``.
