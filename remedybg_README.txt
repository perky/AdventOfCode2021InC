RemedyBG : a user mode 64-bit Windows debugger written from scratch.

Contact Information
-------------------
George Menhorn
remedybg@protonmail.com
https://remedybg.handmade.network/
https://remedybg.itch.io/remedybg
https://github.com/x13pixels/remedybg-issues/issues
https://discord.gg/hxWxDee : handmade.network Discord server (channel #remedybg)

Thank you for purchasing a copy of RemedyBG and supporting its continued development.

Also, thanks to Visual Studio for providing the motivation, in the form of aggravation and want, for
making we want to write a debugger from scratch in the first place.

A big thanks to Casey Muratori for his inspiration, suggestions, and the rekindling of the handmade
movement!

Thanks to Simon Anciaux for the extraordinarily detailed feedback on pretty much every release that
I've put out to date along with many, many others that have provided excellent feedback and feature
suggestions.

I welcome any and all bug reports, feature requests, praise, and/or curses (though please refrain
from using any voodoo). Also, feel free to use the Github issue tracker, listed above, to report
any problems you might bump into.

Usage
-=-=-
 remedybg.exe [exe-filename | rdbg-filename]

 Executing remedybg.exe without any command-line parameters will launch RemedyBG with an unnamed
 session. You can fill in the necessary details for launching an executable in the Session menu.

 exe-filename: Specify an executable to debug. This will auto-populate the session's "Command".

 rdbg-filename: Specify an .rdbg session file to load. This will reload the "Command", "Command
 Arguments", "Working Directory", any files that were opened, and any breakpoints that were set.

 Known issues
 ------------
 RemedyBG does not support loading symbols from binaries created with MSVC's /DEBUG:fastlink switch.
 In the compiler shipped in Visual Studio 2017 (15.0) and later specifying /DEBUG will default to
 /DEBUG:fastlink. This will need to be changed to /DEBUG:FULL, instead.

Driving an existing instance
-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   An existing instance of RemedyBG can be driven via the command-line. Following are the currently
   supported commands and their arguments. Optional arguments are enclosed in brackets '[]'.

   Note that filenames passed in this fashion should, at the moment anyway, be absolute and not
   relative.

   remedybg.exe open-session session-filename
   remedybg.exe open-file filename [line-number]
   remedybg.exe close-file filename
   remedybg.exe start-debugging
   remedybg.exe stop-debugging
   remedybg.exe attach-to-process-by-id process-id [--continue]
   remedybg.exe continue-execution
   remedybg.exe add-breakpoint-at-file filename line-number
   remedybg.exe remove-breakpoint-at-file filename line-number
   remedybg.exe add-breakpoint-at-function function-name [condition expression]
   remedybg.exe remove-breakpoint-at-function function-name
   remedybg.exe run-to-cursor filename line-number

Sessions
-=-=-=-=
   A session in RemedyBG is a simple binary file that, presently, contains information about the
   command, command arguments, working directory, any source files that were opened, along with any
   breakpoints that were set.

   From the 'Session' menu you can either open a saved session (.rdbg file) or enter the command,
   command arguments (if any), and working directory (if different than the command's path). 

   You can set environment variables in the same menu. Variables are parsed from the given input as
   VARNAME=VARVALUE, one per line. Lines that are not in this form are ignored.  There is also an
   option for inheriting the parent environment variables.

   Multiple configurations
   -----------------------
   Multiple configurations can be created that share the same breakpoints, watch expressions, and so
   forth but with different commands, command arguments, working directory, and environment.  Within
   the Application and Parameters block of the session menu there are buttons that can be used to
   cycle between the active configurations, create a new configuration (cloned from the active
   configuration), and delete configurations. Configurations can be quickly changed by using
   the Ctrl+Shift+1, ... Ctrl+Shift+9 hotkeys.

Storage
-=-=-=-
   Files for storing UI state, and so forth, are located in the current user's application data
   directory.  You can get to this directory in Windows explorer by typing "%APPDATA%\remedybg". In
   the case that the smoke hits the fan and you want to reset RemedyBG to its default state, you can
   delete the files in that directory (or simply delete the entire remedybg folder).

   If you prefer your settings to be stored in an alternate location, you can start RemedyBG with
   the "--settings" parameter and specify the alternate folder. For example:

      remedybg --settings C:\your\settings\folder

Text Window
-=-=-=-=-=-
   General
   -------
   Note that any opened text windows are saved as part of the session (.rdbg) file.

   Search
   ------
   Ctrl+F will initiate a search in the current file. If the entered text contains all lower-case
   characters, then the search is case-insensitive. Otherwise, the search is case-sensitive. F3 can
   be used to repeat the last search (forward) or Shift+F3 (reverse). Any text selected in the
   active window will be used to initialize the search pattern. Speaking of patterns, regular
   expression support is not currently implemented.

Watch Window
-=-=-=-=-=-=
   Registers
   ---------
   The watch window can parse register names such as RAX, RIP, EAX, AL, XMM0, YMM4, and so forth.
   The register names are case insensitive (e.g., Rax or rax work as well). Note that variables in
   scope take precedence over a register name. So, for example, if you had a local variable name
   rsp, then the watch window will show that variable's value and not the register name. To override
   any variable shadowing, you can prefix the variable name with a '$' (e.g., $RAX).

   Relative Scopes
   ---------------
   A watch window expression can indicate the relative scope for which to perform a variable lookup.
   The syntax for this uses a prefix of the form "{n}" or "{^n}" where n >= 0. For n = 0, this is
   the normal lookup at the current scope. For n > 0, the resolution of an identifier is done 'n'
   frames above the current frame. In addition, you can use "{*}" or "{^*}" to search up the stack
   to the first frame that properly resolves an identifier. Some examples of this would look
   like:

       {1}foo          # Resolve 'foo' in the parent frame
       {*}(char*)&foo  # Resolve expression in first ancestor containing the identifer 'foo'

   Locking Scope
   -------------
   For an expression in the watch window you can toggle the lock icon. Locking an expression will
   maintain the scope of expression.

   Format Specifiers
   -----------------
   A format specifier can alter the formatting, or display, of the value of an expression in the
   watch window. The syntax of format specifiers is a comma at the end of an expression followed by
   the format specifier. Valid format specifiers are the following:

   -- b : Using the 'b' format specifier will cause the output of any values to be in binary,
   rather than decimal.

       var, b           # Display 'var' and any children in binary

   -- d: Using the 'd' format specifier will cause the output of any values to be in decimal (the
   default). This can be used as an override in the case that a watch window is set to display all
   values in hexadecimal.

   -- hr: The 'hr' format specifier will append an HRESULT or Win32 error code message

       0x80004003, hr   # Displays "0x80004003: Invalid pointer"
       $err, hr         # Displays the HRESULT or Win32 error code of the last error

   -- x or X : Using the 'x' format specifier will cause the output of any values to be in hexadecimal,
   rather than decimal. Note that this format specifier takes precedence over the specifier for
   number of elements. Similarly, using 'X' will display upper-case hexadecimal values where
   appropriate.

       var, x           # Display 'var' and any children in lower-case hexadecimal
       var, X           # Display 'var' and any children in upper-case hexadecimal

   -- <num elements>: This is for specifying the number of elements to display in an array or pointer
   type. The value can be an integer literal or an integer/enum value that can be resolved in the
   current scope. The <num elements> expression can be used in conjunction with another format
   specifier.

       arr, 16            # Display 16 values of the expression 'arr'. 
       arr, count         # Display 'count' values of the expression 'arr'. 
       arr, X count       # Display 'count' values of the expression 'arr' in upper-case hexadecimal

   -- str or wstr : Using the 'str' or 'wstr' format specifier will show an inline, multiline string
   view in the watch window. This format specifier can be followed with a value to indicate the
   number of characters to display. The value can be an integer literal or an integer/enum value
   that can be resolved in the current scope. Use 'wstr' for wide character strings.

      ShaderText, str             # Displays the contents of 'ShaderText' in an inline string view
      text.Data, str text.Count   # Displays 'text.Count' characters of 'text.Data' in an inline string view

   -- _ : A no-op format specifier that can be used to disambiguate between a format specifier and a
   local variable used as a <num elements> expression.

      arr, x              # Display array 'arr' in lower-case hexadecimal
      arr, _ x            # Display 'x' values of the expression 'arr'
      arr, x x            # Display 'x' values of the expression 'arr' in lower-case hexadecimal

   -- vector register/type format specifiers
      (xmmN|ymmN|zmmN), ps                                # Display vector register/type as floats
      (xmmN|ymmN|zmmN), pd                                # Display vector register/type as doubles

      (xmmN|ymmN|zmmN), (epi8|epi16|epi32|epi64)          # Display vector register/type as signed values
      (xmmN|ymmN|zmmN), (epu8|epu16|epu32|epu64)          # Display vector register/type as unsigned values
      (xmmN|ymmN|zmmN), (epu8x|epu16x|epu32x|epu64x)      # Display vector register/type as unsigned
                                                            values in lower-case hexadecimal
      (xmmN|ymmN|zmmN), (epu8X|epu16X|epu32X|epu64X)      # Display vector register/type as unsigned
                                                            values in upper-case hexadecimal

   -- inline-memory format specifiers. These take the form:

         address_expr, <format specifier> [/cColumnCount] [(/nNumObjects|ending_address_expr)]

      format specifiers: (note that the names of these specifiers were borrowed from WinDBG)

         db  : display byte values along with ASCII characters
         dc  : double-word values and ASCII characters
         dd  : double-word values
         dD  : double-precision floating-point numbers
         df  : single-precision floating-point numbers
         dq  : quad-word values
         dW  : word values and ASCII characters
         dw  : word values
         dyb : binary values and byte values
         dyd : binary values and double-word values

         /c  : used to specify the number of columns (e.g., /c16). If not specified, then a suitable
               default for the format specifier is used.

         /n  : used to specify the number of objects (bytes, words, double-words, quad-words, floats, or
               doubles); e.g, /n12. Cannot be used if ending_address_expr is given.

         ending_address_expr : any expression that can be evaluated to an address. Cannot be used at
               the same time as the '/n' option.

   -- matrix format specifiers. Note that you can change between row major (the default) and column
   major by right-clicking in the watch window and choosing "Column Major Display". These take the form:

          address_expr, <format specifier>

      format specifiers:

         mtx    : display 4 x 4 matrix (floating point values read as row-major)
         mtx4x3 : display 4 x 3 matrix (floating point values read as row-major)
         mtx3x4 : display 3 x 4 matrix (floating point values read as row-major)

   Anonymous Namespaces
   --------------------
   You can use the prefix `anonymous namespace':: to watch values within an anonymous namespace.

   Pseudo Variables
   ----------------
   -- $err (unsigned int): the per-thread value of the last error set by SetLastError
   -- $handles (unsigned int): the number of handles allocated by the process
   -- $tid (unsigned int): the thread ID of the current thread
   -- $pid (unsigned int): the process ID

   Synthetic Structures
   --------------------
   Synthetic structures are identifiers that can be used in the watch window that aren't actual
   types in the target being debugged per se but are, instead, synthetized from the current state of
   the target. Valid synthetic structures are as follows:

   -- __locals : Displays the list of local variables starting from the innermost scope out to
   function scope.  Note that __locals can be combined with a relative scope operator to view local
   variables in parent scopes.

      __locals       # Displays local variables in the current scope
      {^1}__locals   # Display local variables in the parent scope

   -- __params : Displays the list of function parameters. Note that __params can be combined with
   a relative scope operator to view function parameters in parent scopes.

      __params       # Displays the current function's parameters
      {^1}__params   # Displays the parent function's parameters

Breakpoints
-=-=-=-=-=-=
   Processor Breakpoints (or Data Breakpoints as they are known in some circles)
   ---------------------
   A new processor breakpoint can be added in the breakpoints windows. Right-click anywhere in the
   window and choose "New Processor Breakpoint...".

   Note that Intel processors support up to 4 processor breakpoints per thread. At the moment,
   RemedyBG does not take advantage of per-thread processor breakpoints and limits the number to 4.
   
   Conditional Breakpoints 
   -----------------------
   Conditions on any type of breakpoint can be added by right-clicking on a breakpoint (or a
   breakpoint icon in a text window) and choosing "Edit". The condition can be any valid expression
   that can be evaluated to a boolean.


Keyboard Shortcuts
-=-=-=-=--=--=-=-=

   Ctrl+Alt+[0-9]       Switch to a saved layout

   Ctrl+Tab             Switch active text window
   Ctrl+Alt+B           Display the breakpoints window
   Ctrl+Alt+C           Display the call stack window
   Ctrl+Alt+D           Display the disassembly window
   Ctrl+Alt+M           Display the first memory window
   Ctrl+Alt+O           Display the output window
   Ctrl+Alt+R           Display the registers window
   Ctrl+Alt+T           Display the threads window
   Ctrl+Alt+U           Display the modules window; cycles through open modules windows
   Ctrl+Alt+W           Display the first watch window; cycles through open watch windows
   Esc                  If disassembly window is in focus, then Esc returns focus to topmost text window.
                        Otherwise, returns focus to either disassembly or text window, whichever one
                        was in focus last.

   Ctrl+B               Add function breakpoint
   Ctrl+Shift+X         Break execution.
   F12                  Break execution when the application being debugged is in focus. This hotkey
                        allows you to break execution without having to change back to RemedyBG
                        first. Note that this is a Windows system-wide hotkey and can be changed by
                        modifying the value of the following registry key:
                           HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug; UserDebuggerHotKey (REG_DWORD)
                        See Microsoft's documentation for further details.
   F5                   Start debugging / continue execution
   Shift+F5             Stop debugging
   Ctrl+Shift+F5        Restart
   Shift+F6             Attach to process (when no target)
   Shift+F6             Detach (if target is executing)
   Ctrl+Shift+F6        Reattach to process
   F9                   Toggle breakpoint in text window
   Ctrl+F9              Toggle breakpoint enabled in text window
   F10                  Step Over (Line); when disassembly window is active, this behaves as Step Over (Instruction)
   Ctrl+F10             Run to Cursor (can be used even when the target has not been launched)
   Ctrl+Shift+F10       Set Next Statement
   F11                  Step Into (Line); when disassembly window is active, this behaves as Step In (Instruction)

   Ctrl+G               Brings up the Go To Line popup for the active text window
   Ctrl+F               Brings up the Find in File popup for active text window
   F3                   Repeat last search (forward)
   Shift+F3             Repeat last search (reverse)
   Ctrl+W               Close current source file

   Ctrl+O               Open source file
   Ctrl+Shift+O         Open session
   Ctrl+S               Save session
   Ctrl+Shift+[1-9]     Select active configuration

   Ctrl+A               Select All

   PgDown               Scroll down
   PgUp                 Scroll up
   Home                 Scroll to top
   End                  Scroll to bottom

   -- Breakpoints window
   Ctrl+C               Copy information on selected row(s) to clipboard
   Del                  Delete currently selected breakpoint(s)
   Enter                Go to location of breakpoint in text window
   Alt+Enter            Edit the selected breakpoint
   Space                Toggle breakpoint(s) enabled

   -- Callstack window
   Enter                Set the current frame
   Ctrl+Up              Change to the next frame
   Ctrl+Down            Change to the previous frame

   -- Output window
   Ctrl+C               Copy contents of output window to clipboard

   -- Threads window
   Ctrl+C               Copy information on selected row(s) to clipboard
   Enter                Set the active thread

   -- Watch window
   Ctrl+Up              Move watch up
   Ctrl+Down            Move watch down
   F2,Shift+Enter       Edit watch expression
   Enter                Toggle fold state between expanded and collapsed


Attribution
-=-=-=-=-=-

RemedyBG would not be possible without the following:

 * Dear ImGui: https://github.com/ocornut/imgui, docking branch 7b53551032
 * Omar's mini memory editor for ImGui: https://github.com/ocornut/imgui_club/tree/master/imgui_memory_editor
 * Intel XED: https://github.com/intelxed
 * Heavily modified ImGuiColorTextEdit from https://github.com/BalazsJako/ImGuiColorTextEdit used as
 a starting point for the text view.

Changelog
-=-=-=--=

Version 0.3.6.2
---------------
MD5: 9560103d7c1147f651f3b3955e623e3f
SHA1: 20c27dfb3b5ee97ea0eafd2006b3b1d969130b4d
Improvements
 * Allow toggling the display of module load/unload, thread exit, and process exit messages

Version 0.3.6.1
---------------
MD5: d8e44cb18ff3aea47425e63c62b8591b
SHA1: c4b118205dfbfd4ab96b9a9c4eb79f39c7ca6b78
Bug Fixes
 * Fixes potential crash in stack walk code introduced in 3.6.0 (The Sandvich Maker)
 * Evaluate on hover will now consider array indexing when picking the expression to evaluate
 (datamite)
 * Fixes conditional breakpoints added at program startup (datamite)
 * AltGR+[^A-Z] can now be used to initiate an edit in the watch window (seventh-chord)
 * Fixed expression generated for Add Watch / Send to Memory Window for dereferenced pointers
 (JensWallgren)

Version 0.3.6.0
---------------
MD5: 3087556dc2eb80e45560d82bae5a3689
SHA1: 07d7d809da03aa6ca9bae7a84e9837e7615e79f0
New Features
 * Evaluate on hover. Can set the tooltip delay in Windows>Style and can optionally disable this
 feature altogether.
 * A stack-overflow exception will now collapse repeated frames and show "Collapsed N frames" in the
 Call Stack pane. Can double-click collapsed frame or toggle this behavior via "Collapse Repeated
 Frames" context menu item.
 * Adds "Format As" menu item showing relevant format specifiers for the selected watch(s).
 * Added $err pseudo-variable for the last error value set by SetLastError for the current thread
 * Added $handles pseudo-variable which shows the number of handles allocated by the process
 * Added $tid and $pid pseudo-variables for thread and process IDs
 * Added ",d" decimal format specifier
 * Added ",hr" format specifier for displaying an HRESULT or Win32 error code
 * Adds process ID (optionally) to status text. Can be toggle via status text's context menu.
 * Information about module load/unload events and thread exit codes are now written to the Output window
 * Added "Read Only" checkbox to memory window's options
 * Added "Copy Address" to memory window's context menu
 * Allow breakpoint on either nominal entry point (WinMain, main, etc...) or entry point as defined
 in the PE header.
 * Can now run to a filename/line via the command line driver (see run-to-cursor in documentation)

Improvements
 * Lazily load function table entries. This helps improve performance when stepping over calls that
 load many DLLs (e.g., Direct3D initialization).
 * Use high-contrast color for collapsed panes to make them easier to find.
 * Adds error message when trying to index non-array/pointer type.
 * Properly handle device removed scenarios (Direct3D).
 * Line numbers have been added back to the call stack display.
 * Render the frame rather than show black while resizing.
 * Implements PgUp/PgDown in panes (previous these keys mapped to what is Home/End now)
 * Added binary formatting of enum values
 * We now alphabetically sort text file names in the drop-down menu.
 * Adds Ctrl+C hotkey to Output Window to copy contents to clipboard
 * Rearranged menu in Output Window so "Clear" is less likely to be accidently clicked.
 * Adjusted vertical position of lock icon in embedded font
 * Added more descriptive error message in the case CreateProcess fails
 * No longer count disable processor BPs against total number use
 * Adds "Move Up / Move Down" menu items in watch window to aid discoverability of this existing
 feature.
 * Show arrow next to processor breakpoint that was hit.
 * Run-to-cursor operation will now show yellow breakpoint symbol in text/disassembly
 * No longer initiate editing of address in memory window if a cell is being edited.
 * Double-click, not single-click, now used to initiate memory window editing.
 * An error in a breakpoint's condition is now more clearly presented.
 * Allow adding a breakpoint at an address without having to open/navigate to the address in the
 disassembly window.

Bug Fixes
 * Fixed lookups of static, global TLS variables.
 * Fixes the case in which a breakpoint is added on the same line as the run-to-cursor location
 before the run-to-cursor operation completes.
 * String summaries are now culled to 64-chars for non-nested previews and 32-chars otherwise.
 * Addressed problems where double-clicks were not registering on certain touchpads.
 * Fixed a problem where the corresponding text window would not be brought to the front when
 stepping through disassembly.
 * Fixed a case where the session would be considered modified immediately after opening.
 * Fixes a problem where keyboard input was ignored in the auto-created watch expression in certain
 situations.
 * Now properly sign-extend bitfield values.
 * Fixed the editing of bitfield values in watch.
 * An exception is now considered "handled" after using set-next-statement
 * Fixed so that Ctrl+Alt+W no longer deletes a watch expression when changing focus to another
 watch window.
 * Modifying condition on an existing processor breakpoint was broke in previous release. This has
 been fixed.
 * Fixed dereferencing union type when using '.' rather than '->'
 * Left arrow was unintentionally changing to focus to last row in some cases. This has been fixed.
 * Fixed reported case where cursor position on ASM source goes to PROC declaration rather than
 first instruction.
 * Fixed problem terminating app when breakpoint present in multithread code.
 * Fixed bug when run-to-cursor is executed on the same line
 * Fixed hang adding a function breakpoint to a module containing only a single function.
 * Fix for memory window so navigate to an address occurs even when reevaluation of an expression is
 identical to the previous address.


Version 0.3.5.4
---------------
MD5: 8672afff37a5ff944a57d4b59098d0fa 
SHA1: 15056eff0dd52204f975132525587bb84d075cd7
Improvements
 * Slightly better error message for pointer subtraction when the element types are of two different
 sizes.
Bug fixes
 * Remove debug code left in that could cause a crash
 * Fix subtle bug in allocator that could cause a crash in rare cases
 * Fixed crash when using "wstr" formatter with zero length
 * Fixed issue where changing font size would reset dark/light mode setting.

Version 0.3.5.3
---------------
MD5: 45bcc2deffecd5ed80a37c5d10c64cbf
SHA1: aee805b10c224cc9cec436aa73f57c8a58ed71a6
Improvements
 * Ctrl+Up/Ctrl+Down in callstack window will now set the frame relative to the current frame rather
 than relative to the focused row.
 * Expression parser now accepts the less verbose {n} or {*}, as opposed to {^n} or {^*} (caret no
 longer required), when specifying a relative scope.

Bug fixes
 * Fixes a crash triggered when adding or removing a conditional or processor breakpoint while the
 target is running. 
 * An explicit binary format specifier will now take precedence over the global hex display setting.
 * Current breakpoint indicator (arrow) now properly cleared when continuing execution.
 * Set modified bit on session if active configuration is changed.
 * Make sure breakpoint hit "animation" is triggered in disassembly window.

Version 0.3.5.2
---------------
MD5: c73bc1b5e725aa68292d3cfadb8cb8e3
SHA1: 4090d7537fc9475e5fce900745fd60425460dcee
Bug fixes
 * Fixes one off in handling of OutputDebugStringW
 * Fixes infinite loop when obtaining field information for a type that contains a virtual base
 class.

Version 0.3.5.1
---------------
MD5: bc0f39a7ee4b29d52fb33679e95fb5a2 
SHA1: 2559e28b75176098e5abc9275bc584bae4356ea4 
Improvements
 * Support Unicode strings output with OutputDebugStringW.
 * Now show string summary for char[0] as if it were of type char*.
 * The "attach to process" flow is now possible without a mouse.
 * Can now select multiple source files in file open dialog.
 * Now allow subtraction of pointers of differing types as long as the underlying type has the same
 size.
 * Longer string preview shown when not part of an aggregate summary.

Bug fixes
 * Fixed formatting of address value in address breakpoint dialog box
 * Fixed a potential hang when inspecting a global variable which occurs in two or more translation
 units. In this case, we render a synthetic var with the fields name __g0, __g1, and so forth,
 corresponding to each instance of the global variable.
 * Fixed problem with Reattach to Process wouldn't work in some cases.
 * Using space in search term no longer causes a case-sensitive search.
 * Properly format negative signed character literals.
 * Fixed problem where enums couldn't be used as array indices.
 * Fix to allow manipulating diassembly view settings (show address/code bytes) after target process
 is stopped.
 * Fixed evaluation of struct addresses passed by value that were less than eight bytes and not a
 power of two.

Version 0.3.5.0
---------------
MD5: 712b9f95196b2071bee96b3a43d7e097
SHA1: 929902f0a5a8c43923f506c57f2e488e2720d534
New features
 * Added an optional toolbar for debug control. This can be toggled in Window > Style > Show Toolbar
 * Local and global implicit thread local storage variables can now be watched.
 * Adds support for AddressSanitizer. Access violations that attempt to read/write into
 AddressSanitizer's address space (e.g., used to expand virtual address space) are now skipped
 (i.e., allowed to be handled without halting execution). There is no UI required as this is
 detected and handled automatically by RemedyBG. 
 * Added "Reevaluate Automatically" option to memory window to allow reevaluating and updating the
 address in a memory window.

Improvements
 * Allow making forward progress on first-chance, continuable exceptions
 * Properly unwind stack in the case that a null function pointer is called.
 * Bake in information about stdint types (uint8_t, etc.) as a temporary workaround until typedef
 information is properly supported.
 * UI will now be automatically scaled based on the DPI of the monitor.

Bug fixes
 * Fix to properly render unprintable escape characters (e.g., '\n') in watch
 * Fixed evaluation of variables cast to structures in certain cases
 * Fixed output of process ID in the output window.
 * Selection of rows in breakpoints pane using mouse has been fixed
 * Fixed ambiguity of format expression when variable name matches one of the format specifiers.
 * Fixed Edit Breakpoint, Add Watch, and Set Next context-menu handling in the disassembly window.
 * Copying a multiline string value (eg., "var, str") will now be copied to the clipboard as
 expected.

Version 0.3.4.1
---------------
MD5: 407b8ca5e4b3e04fda14c10c02757a19 
SHA1: 399774ffe57faaed7f5618df379c48cc24d6eb4b 
 * Fixes potential crash when displaying long summaries (L. Reyes)

Version 0.3.4.0
---------------
MD5: 74b003e6143a7e8d5db726534371fce7
SHA1: 83cd0e0a90d47eeb36b9f713710dab115f417667
New features
 * Adds column major option to watch windows (context menu toggle)
 * Adds an "Add Watch" context menu item for text windows
 * Allow writing member access operator as '.' instead of '->' even for pointer types.
 * Adds optional Comment column in watch window to allow entering arbitrary text (single line at the
 moment)

Improvements
 * We now save hexadecimal setting for watch windows to settings file
 * Allow repeatedly using Run to Cursor on the same line
 * UI tweaks: reduced size of arrow column in tables, removed unnecessary window padding around
 tables, removes rounding from selection rect in text window
 * Adds numpad keys to list of keys that initiate watch editing
 * Better formatting of array elements (removes redundant expression, align index)
 * We now do automatic type conversion when modifying values in watch.

Bug fixes
 * Prevent potential crash when muting the only active thread.
 * Fix so that clicking scrollbar in table does not try to select row in table.
 * Fixes bug updating current breakpoint when another breakpoint is deleted
 * Fixed potential crash from non-escaped characters in structure summary.
 * Fix to ensure window is updated/redrawn in response to a OutputDebugString debug event
 * Removed hack that disabled modifier keys for a short period of time. This fixes problems with
 repeat-rate and eaten keys on certain keyboard layouts.
 * Fixed bug where size of ",str" format specifier would get ignored
