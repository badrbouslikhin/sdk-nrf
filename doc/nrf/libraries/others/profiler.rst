.. _profiler:

Profiler
########

.. contents::
   :local:
   :depth: 2

The Profiler provides an interface for logging and visualizing data for performance measurements, while the system is running.
You can use the module to profile :ref:`event_manager` events or custom events.
The output is provided using RTT and can be visualized in a custom Python backend.

See the :ref:`profiler_sample` sample for an example of how to use the Profiler.

Configuration
*************

Apart from standard configuration parameters, there is one required setting:

:kconfig:`CONFIG_PROFILER`
  Set this option to add the Profiler source code to the application.
  If you use the Event Manager, you can also set this option by selecting :kconfig:`CONFIG_EVENT_MANAGER_PROFILER`.

Call :c:func:`profiler_init` during the application start to initialize the Profiler.
If you set :kconfig:`CONFIG_EVENT_MANAGER_PROFILER`, the Profiler is automatically initialized when you initialize the :ref:`event_manager`.


Profiling custom events
***********************

To profile custom events, you must register them using :c:func:`profiler_register_event_type`.

The following code example shows how to register event types::

	static const char * const data_names[] = {"value1", "value2", "value3", "value4", "string"};
	static const enum profiler_arg data_types[] = {PROFILER_ARG_U32, PROFILER_ARG_S32,
						       PROFILER_ARG_S16, PROFILER_ARG_U8,
						       PROFILER_ARG_STRING};

	no_data_event_id = profiler_register_event_type("no data event", NULL,
							NULL, 0);
	data_event_id = profiler_register_event_type("data event", data_names,
						     data_types, 5);

After registering the types, you can send information about event occurrences using the following functions:

* :c:func:`profiler_log_start` - Start logging.
* :c:func:`profiler_log_encode_uint32` - Add 32-bit unsigned integer connected with the event (optional).
* :c:func:`profiler_log_encode_int32` - Add 32-bit integer connected with the event (optional).
* :c:func:`profiler_log_encode_uint16` - Add 16-bit unsigned integer connected with the event (optional).
* :c:func:`profiler_log_encode_int16` - Add 16-bit integer connected with the event (optional).
* :c:func:`profiler_log_encode_uint8` - Add 8-bit unsigned integer connected with the event (optional).
* :c:func:`profiler_log_encode_int8` - Add 8-bit integer connected with the event (optional).
* :c:func:`profiler_log_encode_string` - Add string connected with the event (optional).
* :c:func:`profiler_log_send` - Send profiled data.

It is good practice to wrap the calls in one function that you then call to profile event occurrences.
The following code example shows a function for profiling an event with data::

	static void profile_data_event(uint32_t val1, int32_t val2, int16_t val3,
				       uint8_t val4, const char *string)
	{
		struct log_event_buf buf;

		profiler_log_start(&buf);
		/* Profiling data connected with an event */
		profiler_log_encode_uint32(&buf, val1);
		profiler_log_encode_int32(&buf, val2);
		profiler_log_encode_int16(&buf, val3);
		profiler_log_encode_uint8(&buf, val4);
		profiler_log_encode_string(&buf, string);
		profiler_log_send(&buf, data_event_id);
	}

.. note::

	The event ID and the data that is profiled with the event must be consistent with the registered event type.
	The data for every data field must be provided in the correct order.


Supported backends
******************

The Profiler supports a custom backend to visualize the output data.
The backend communicates with the host using RTT.

Custom backend
==============

Select the custom backend to use dedicated tools written in Python for event visualization, analysis, and calculating statistics.

To save profiling data, the tools use CSV files (for event occurrences) and JSON files (for event descriptions).
The scripts can be found under :file:`scripts/profiler/` in the |NCS| folder structure.

Set :kconfig:`CONFIG_PROFILER_NORDIC` to enable this backend.

To use the tools, run the scripts on the command line:

* ``python3 data_collector.py 5 test1``

  Connects to the device using RTT, receives profiling data, and saves it to files.
  As command-line arguments, provide the time for collecting data (in seconds) and a dataset name.

* ``python3 plot_from_files.py test1``

  Plots events from the dataset that is provided as the command-line argument.

* ``python3 real_time_plot.py test1``

  Connects to the device using RTT, plots data in real-time, and saves the data.
  As command line arguments, provide a dataset name.

* ``python3 merge_data.py test_p sync_event_p test_c sync_event_c test_merged``

  Combines data from test_p and test_c datasets into one dataset (test_merged).
  Provides clock drift compensation based on synchronization events: sync_event_p and sync_event_c.
  This enables you to observe times between events for the two connected devices.
  As command-line arguments, provide names of events used for synchronization for a Peripheral (sync_event_p) and a Central (sync_event_c), as well as names of datasets for: the Peripheral (test_p), the Central (test_c), and the merge result (test_merged).

Visualization
-------------

When running ``plot_from_files.py`` or ``real_time_plot.py``, the profiled events are visualized in a GUI window.

When displaying Event Manager events, submissions are marked as dots.
Processing of the events is displayed as rectangles, visualizing the processing time.

Use the :guilabel:`start/stop` button below the plot to pause or resume real time plot translation.
Scroll to zoom in or out.
When paused, scrolling zooms to the cursor location.
When plotting in real-time, scrolling zooms to the right edge of the plot.
Use the middle mouse button to mark an event submission or processing for tracking, and to display the event data.

When plotting is paused, you can click and drag with the left mouse button to pan the plot.
Click the left or right mouse button to place a vertical line at the cursor location.
When two lines are present, the application measures the time between them and displays it.


Shell integration
*****************

The Profiler is integrated with Zephyr's :ref:`zephyr:shell_api` module.
When the shell is turned on, an additional subcommand set (:command:`profiler`) is added.

This subcommand set contains the following commands:

:command:`list`
  Show a list of profiled event types.
  The letters "E" or "D" indicate if profiling is currently enabled or disabled for a given event type.

:command:`enable` or :command:`disable`
  Enable or disable profiling.
  If called without additional arguments, the command applies to all event types.
  To enable or disable profiling for specific event types, pass the event type indexes (as displayed by :command:`list`) as arguments.


API documentation
*****************

| Header file: :file:`include/profiler.h`
| Source files: :file:`subsys/profiler/`

.. doxygengroup:: profiler
   :project: nrf
   :members:
