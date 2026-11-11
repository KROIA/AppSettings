# Settings

## Usage

### Key sequence settings
```cpp
AppSettings::Setting m_rotate = AppSettings::Setting("Rotate", QKeySequence("Ctrl+R"));
AppSettings::Setting m_comment = AppSettings::Setting("Comment", QKeySequence("Ctrl+K, Ctrl+C"));
```
A `QKeySequence` is saved as portable text, e.g. `"Ctrl+K, Ctrl+C"`. An empty sequence means "no shortcut".
The editor shows a key capture field with a clear button.

### Types are kept on load
```cpp
AppSettings::Setting m_count = AppSettings::Setting("Count", 5);
settings.load(); // m_count still holds an int, not a double
```
`load()` converts the stored value to the type of the setting's current value.
If that is not possible without losing data, the loaded value is kept as it is and a warning is logged.

### Default values and reset
```cpp
m_count.setValue(7);
m_count.isDefault();          // false
m_count.resetToDefault();     // back to 5, emits valueChanged()
m_group.resetToDefault();     // all settings of the group and its subgroups
m_group.resetToDefault(false);// only the settings of this group
```
The default is the value the setting was constructed with (also for `ListSetting` and `MapSetting`).
In the editor, every setting has a reset button and "Reset group" resets the shown group. The changes are applied on Save and discarded on Cancel.

### Validation before save
```cpp
m_shortcuts.setValidator([this](const AppSettings::SettingsGroup&) -> QString
{
    if (m_rotate.getValue() == m_comment.getValue())
        return "Rotate and Comment use the same shortcut";
    return QString(); // valid
});
QString message = m_shortcuts.validate(); // runs the validators of the group and its subgroups
```
On Save, the editor validates the edited values of all groups first.
If a validator returns a message, it is shown, nothing is saved and the editor stays open.

### Filter in the editor
The filter field above the settings list shows only the settings whose name or saved value contains the typed text (case insensitive).

### Translations
The UI texts are German. Call `installTranslator()` after creating the `QApplication` to show them in another language:
```cpp
AppSettings::installTranslator();                           // system language
AppSettings::installTranslator(QLocale(QLocale::English)); // fixed language
```
For a German locale the German texts are shown. If there is no translation for the locale, English is used and `installTranslator()` returns false.
The translations are embedded in the library, nothing has to be deployed.

To add a language:
1. Copy `core/translations/appsettings_en.ts` to `appsettings_<language>.ts` (e.g. `appsettings_fr.ts`) and set `language="fr"` in the `TS` tag.
2. Translate it, e.g. with Qt Linguist.
3. Add the file to `TS_FILES` in `core/CMakeLists.txt`.

After changing UI texts, update the `.ts` files with the `AppSettings_lupdate` target (it never runs during a normal build):
```
cmake --build build/x64-Debug --target AppSettings_lupdate
```
