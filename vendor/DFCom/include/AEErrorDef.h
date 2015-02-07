//! \file AEErrorDef.h
//! \brief Fehlerdefinitionen.
//////////////////////////////////////////////////////////////////////

#ifndef _AEERRORDEF_H_
#define _AEERRORDEF_H_

// CChannelBase
#define IV_ERROR_SUCCESS               0 // Funktion erfolgreich durchgefuehrt
#define IV_ERROR_FAIL                  100 // Funktionsausfuehrung fehlgeschlagen.
#define IV_ERROR_CHANNEL_OPEN          2 // Schnittstelle des Kanals konnte nicht geoffnet werden.
#define IV_ERROR_CHANNEL_OPEN_REMOTE   2 // Schnittstelle wurde von Remote geschlossen. (Nur TCP/IP)
#define IV_ERROR_CHANNEL_CLOSED        3 // Schnittstelle des Kanals ist geschlossen
#define IV_ERROR_CHANNEL_COMMUNICATION 4 // Allgemeiner Fehler bei der Kommunikation aufgetreten.
#define IV_ERROR_RECEIVE               5 // Ein Zeichen konnte nicht gelesen werden.
#define IV_ERROR_CHANNEL               10 // Kanalobjekt nicht vorhanden oder erstellbar
#define IV_ERROR_TRANSMIT              6 // Ein Zeichen konnte nicht geschrieben werden.
#define IV_ERROR_CHANNEL_USED          2 // Kanal bereits in Verwendung

// CChannel4Base
#define IV_ERROR_CRC_HEAD              1 // CRC - Paketkopf war falsch.
#define IV_ERROR_CRC_PACKET            1 // CRC - Gesamtpaket war falsch.
#define IV_ERROR_CRC_SEGMENT           1 // CRC von uebertragenen Segment falsch.
#define IV_ERROR_PACKETLENGTH          4 // Die Laengenangabe des Pakets liegt auserhalb ihrer Grenzen.
#define IV_ERROR_BREAK                 16 // Abbruch durch Benutzer (CallBack-Funktion)
#define IV_ERROR_NO_REQUEST            4 // Aktive Antwort kam nicht.
#define IV_ERROR_ADDRESS               8 // Die Adressen stimmen nicht ueberein.
#define IV_ERROR_COMMAND               4 // Empfangenes Kommandobyte unbekannt.
#define IV_ERROR_SKIPCOUNT             4 // Anzahl IV_SKIPCOUNT erreicht
#define IV_ERROR_START_IN_PACKET       4 // Startzeichen im gelesenen Datenstrom
#define IV_ERROR_ESCAPE                4 // Gelesenes Escapezeichen waehrend Escapemodus
#define IV_ERROR_ESCAPESEQUENZ         4 // Gelesene Escapesequenz undefiniert
#define IV_ERROR_INTERN                4 // Alles was waehrend der Routinenarbeit nicht auftreten darf
#define IV_ERROR_FUNCTION_UNAVAILABLE  27 // Funktion steht im vorliegenden Fall nicht zur Verfuegung.
#define IV_ERROR_UNREACHABLE           5 // Geraet unerreichbar (Antwortet nicht mehr)

// Zu entfernen
#define IV_ERROR_DEVICE_ID_REMOTE      9
#define IV_ERROR_BOOTVERSION           24

// Zusaetzliche Fehlercodes
#define IV_ERROR_FILE_OPEN             11 // Eine Datei konnte nicht geöffnet oder erzeugt werden.
#define IV_ERROR_FILE_FORMAT           12 // Das Format einer Datei stimmt nicht mit dem geforderten ueberein.
#define IV_ERROR_LIST_PROCESSING       13 // Bei der Verarbeitung von Listendaten trat ein Fehler auf.
#define IV_ERROR_VERSION_FIRMWARE      14 // Firmwareversion ist nicht mit der abzuarbeitenden Routine kompatibel.
#define IV_ERROR_DATA_AVAILABLE        15 // Die geforderte Aktion würde noch vorhandene Daten löschen.
#define IV_ERROR_USER_CANCEL           16 // Die durchzuführende Aktion wurde vom Benutzer abgebrochen.
#define IV_ERROR_DESCRIPTION_LIST_UNLOADED   17 // Für die Verarbeitung werden die Listenbeschreibungen gefordert, diese sind jedoch nicht vorhanden.
#define IV_ERROR_DESCRIPTION_RECORD_UNLOADED 18 // Für die Verarbeitung werden die Datensatzbeschreibungen gefordert, diese sind jedoch nicht vorhanden.
#define IV_ERROR_FUNCTION_PARAMETER    20 // Übergebener Parameter einer Funktion liegt ausserhalb seines gültigen Bereichs.
#define IV_ERROR_VARIABLE_UNDEFINED    21 // Die angegebene Variable ist nicht definiert.
#define IV_ERROR_VARIABLE_VALUE        26 // Der angegebene Wert einer Variablen konnte nicht übernommen werden.
#define IV_ERROR_PROCESSING            28 // Der Befehl konnte zwar übermittelt werden, wurde jedoch nicht ausgeführt.
#define IV_ERROR_DEVICE_UNKNOWN        29 // Der Geraetetyp ist unbekannt. Update der Software erforderlich.
#define IV_ERROR_DEVICEFILE_WRONG      30 // Bei Geraeteupdate stimmt der Geraetetyp nicht mit dem der Geraetedatei ueberein.
#define IV_ERROR_DEVICE_WRONG          31 // Geraetetyp stimmt nicht. z.B. Versuchtes Einspielen von Setupdatei fuer PZE - Master IV in BDE - Master IV
#define IV_ERROR_FILE_EXTENSION        32 // Die angegebene Dateiendung wird nicht unterstuetzt.
#define IV_ERROR_ENTRANCE2_OFF         39 // Die Zutrittskontrolle ist aus.
#define IV_ERROR_OVERFLOW              40 // Bei einer Datenübertragung ist ein Speicherüberlauf eingetreten.
#define IV_ERROR_NO_DATA_AVAILABLE     41 // Es liegen keine Daten vor.
#define IV_ERROR_PROCESSING_DLL        42 // Der Befehl konnte zwar übermittelt werden, bei den Daten liegt jedoch ein fehler vor.
#define IV_ERROR_MODULE_BUSY           43 // Das angesprochene Modul ist zur Zeit durch einen anderen Process blockiert.
#define IV_ERROR_FILE_LOAD_XML         44 // Fehler beim Laden der XML-Datei zu ModifyStudioFile
#define IV_ERROR_FILE_UNKNOWN_XML      45 // Unbekannte Elemente der XML-Datei zu ModifyStudioFile
#define IV_ERROR_FILE_CONTENT_XML      46 // Fehler im Dateninhalt der XML-Datei zu ModifyStudioFile
#define IV_ERROR_FILE_MODIFY           47 // Fehler bei Uebernahme (Anwendung) der zu modifizierenden Daten

// Fehler fuer Listenfunktionen
#define IV_ERROR_NAME_UNKNOWN          33 // Der Tabellenname oder Spaltenname ist undefiniert.
#define IV_ERROR_HANDLE                34 // Ein angegebener Handle ist nicht gueltig.
#define IV_ERROR_NULL_RESULT           35 // Der anzuwendende Filter wuerde keine Datensaetze liefert.
#define IV_ERROR_BOF                   36 // Sprung fuehrte zum Anfang der Tabelle. (Base of File)
#define IV_ERROR_EOF                   37 // Sprung fuehrte zum Ende der Tabelle. (End of File)
#define IV_ERROR_VALUE                 38 // Der angegebene Wert ist Fehlerhaft.

#define IV_ERROR_ALLOC_MEM             48 // Speicherallokation konnte nicht durchgefuehrt werden.
#define IV_ERROR_VERSION_DEVICEFILE    49 // Diese Version der Geraetedatei kann aufgrund ihrer Aktualität nicht verarbeitet werden.
#define IV_ERROR_FILE_PROCESSING       50 // Bei der Verarbeitung der Dateidaten trat ein Fehler auf.
#define IV_ERROR_DEVICEFILE_MATCH      51 // Die vorliegende Geraetedatei passt nicht zur vorliegenden Hardware. Oder aber: Im Archiv wurde keine passende Gerätedatei gefunden.
#define IV_ERROR_DEVICEFILE_LOAD       52 // Fehler beim Laden der Gerätedatei(archive) aufgetreten.

#define IV_ERROR_AC_STARTED            53 // Die Aktive Verbindung ist gestartet.
#define IV_ERROR_AC_STOPPED            54 // Die Aktive Verbindung ist nicht gestartet.
#define IV_ERROR_AC_PORTINUSE          55 // Der angegebene Listensocket ist bereits in Verwendung.
#define IV_ERROR_AC_NO_NOTIFY_RECORD   56 // Die Aktive Meldung von Datenätzen ist deaktiviert.
#define IV_ERROR_AC_LISTENER           57 // Allgemeiner Fehler beim Starten der Aktiven Verbindung

#define IV_ERROR_NEED_PASSWORD         58 // Das angegebene Passwort stimmt nicht mit dem benötigten überein. Es wird ein Passwort gefordert.
#define IV_ERROR_PASSWORD_LOCK         59 // Gerät ist aufgrund von fehlerhaften Passworteingaben für 60 Sekunden gesperrt.

#define IV_ERROR_FILE_WRITE            60 // Das Schreiben in die Datei ist fehlgeschlagen.
#define IV_ERROR_FILE_READ             61 // Das Lesen aus der Datei ist fehlgeschlagen.
#define IV_ERROR_FILE_CRC              62 // Fehlerhafter Dateninhalt. Die Prüfung auf die Datenkonsistenz per CRC schlug fehl.
#define IV_ERROR_MEMORY_FULL           63 // Es steht kein weiterer Speicherplatz zur Verfügung.\nZ. B. kann die maximale aufnehmbare Anzahl Fingertemplates erreicht sein.
#define IV_ERROR_FILE_DATA_FORMAT      64 // Die in der Datei vorliegenden Daten stimmen nicht mit den geforderten überein.\nZ. B. in der Datei liegen Fingertemplates vom Typ Idencom-Compact vor, es werden jedoch DIN Erfordert.
#define IV_ERROR_NOT_ENOUGH_FLASH      65 // Es steht nicht genügend Flash-Speicher zur Verfügung, um die Daten aufzunehmen.
#define IV_ERROR_NO_MODULE             66 // Das von Ihnen angesprochene Modul ist eine Optionale Hardware-Erweiterung und ist im vorliegenden Gerät nicht verfügbar.

#define IV_ERROR_VERSION_SETUPFILE     67 // In der Setupdatei werden Erweiterungen verwendet, die einen aktuelleren Firmwarestand voraussetzen.
#define IV_ERROR_WRONG_FIRMWARE        68 // Die abzuarbeitende Aufgabe wird durch die vorliegenden Firmwareversion nicht unterstützt.
#define IV_ERROR_WRONG_CONFIG          69 // Die abzuarbeitende Aufgabe wird durch die vorliegenden Gerätekonfiguration nicht unterstützt.

// Fehler fuer Lantronix Monitormode
#define IV_ERROR_ENTER_MONITOR         70 // 
#define IV_ERROR_EXIT_MONITOR          71 // 

// Fehlermeldungen der Mehrfachdocking
#define IV_ERROR_WRONG_TIMEBOY_MODEL   77 // Das angesprochene Geraet ist ein TimeboyI-III
#define IV_ERROR_SLOT_NONE             78 // Die angesprochene DeviceID ist kein Slot einer Timeboystation.

#define IV_ERROR_SETUPFILE_MIN_LIBRARY_VERSION             80 // Die Setupdatei setzt eine nicht erreichte Mindestversion der DFComDLL voraus.
#define IV_ERROR_SETUPFILE_SHOULD_LIBRARY_VERSION          81 // Die Setupdatei gibt eine nicht erreichte Soll-Version der DFComDLL vor.
#define IV_ERROR_SETUPFILE_UNSUPPORTED_FIRMWARE1_VERSION   82 // Die Setupdatei wird durch die bekannte Reihe der Firmwareversion 04.01.xx nicht unterstuetzt.
#define IV_ERROR_SETUPFILE_MIN_FIRMWARE1_VERSION           83 // Die Setupdatei setzt eine nicht erreichte Mindestversion der Firmware 04.01.xx voraus.
#define IV_ERROR_SETUPFILE_SHOULD_FIRMWARE1_VERSION        84 // Die Setupdatei gibt eine nicht erreichte Soll-Version der Firmware 04.01.xx vor.
#define IV_ERROR_SETUPFILE_UNSUPPORTED_FIRMWARE2_VERSION   85 // Die Setupdatei wird durch die bekannte Reihe der Firmwareversion 04.02.xx nicht unterstuetzt.
#define IV_ERROR_SETUPFILE_MIN_FIRMWARE2_VERSION           86 // Die Setupdatei setzt eine nicht erreichte Mindestversion der Firmware 04.02.xx voraus.
#define IV_ERROR_SETUPFILE_SHOULD_FIRMWARE2_VERSION        87 // Die Setupdatei gibt eine nicht erreichte Soll-Version der Firmware 04.02.xx vor.

#define IV_ERROR_NEED_USER_PASSWORD                        88 // Fuer die verschluesselte Kommunikation mit dem Geraet wird ein Benutzer-Passwort benoetigt.

#define IV_ERROR_SETUPFILE_UNSUPPORTED_FIRMWARE3_VERSION   89 // Die Setupdatei wird durch die bekannte Reihe der Firmwareversion 04.03.xx nicht unterstuetzt.
#define IV_ERROR_SETUPFILE_MIN_FIRMWARE3_VERSION           90 // Die Setupdatei setzt eine nicht erreichte Mindestversion der Firmware 04.03.xx voraus.
#define IV_ERROR_SETUPFILE_SHOULD_FIRMWARE3_VERSION        91 // Die Setupdatei gibt eine nicht erreichte Soll-Version der Firmware 04.03.xx vor.

#endif
