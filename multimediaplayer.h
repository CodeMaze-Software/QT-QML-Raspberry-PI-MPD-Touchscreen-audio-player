#ifndef MULTIMEDIAPLAYER_H
#define MULTIMEDIAPLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QString>
#include <QQueue>
#include "connector.h"
#include "dataparser.h"

#define STATUS_REQUEST              "status"
#define CURRENT_FILE_INFO_REQUEST   "currentsong"
#define PLAYLIST_INFO_REQUEST       "playlistinfo"
#define LIST_ALL_IN_DIRECTORY_CMD   "listall"

#define VOLUME_CMD          "volume"
#define SET_VOLUME_CMD      "setvol"
#define REPEAT_CMD          "repeat"
#define RANDOM_CMD          "random"
#define SINGLE_CMD          "single"
#define CONSUME_CMD         "consume"
#define PLAYLIST_CMD        "playlist"
#define PLAYLIST_LEN_CMD    "playlistlength"
#define MIX_RAMP_DB_CMD     "mixrampdb"
#define SONG_CMD            "song"
#define SONG_ID_CMD         "songid"
#define ELAPSED_CMD         "elapsed"
#define BITRATE_CMD         "bitrate"
#define AUDIO_FORMAT_CMD    "audio"
#define NEXT_SONG_CMD       "nextsong"
#define NEXT_SONG_ID_CMD    "nextsongid"

#define FILE_CMD            "file"
#define LAST_MOD_CMD        "last-modified"
#define TIME_CMD            "time"
#define DURATION_CMD        "duration"
#define POSITION_CMD        "pos"
#define ID_CMD              "id"

#define STATE_CMD           "state"
#define STOP_CMD            "stop"
#define PLAY_CMD            "play"
#define PAUSE_CMD           "pause"
#define NEXT_CMD            "next"
#define PREV_CMD            "previous"
#define SEEK_CMD            "seek"

#define PLAYLIST_ADD_CMD    "playlistadd"
#define PLAYLIST_SAVE_CMD   "save"
#define PLAYLIST_LOAD_CMD   "load"
#define PLAYLIST_CLEAR_CMD  "playlistclear"
#define PLAYLIST_RMV_CMD    "rm"
#define PLAYLIST_CLR_CMD    "playlistclear"
#define PLAYLISTS_LIST_CMD  "listplaylists"
#define PLAYLIST_LIST_CMD   "listplaylist"
#define PLAY_PLAYLIST_MACRO_CMD "command_list_begin\nclear\nload \"%1\"\nplay\ncommand_list_end"

#define DEFAULT_NEG_VAL     "-1"
#define DEFAULT_VAL         "0"
#define DEFAULT_TRUE        "1"


class MultimediaPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int currentVolume READ getCurrentVolume NOTIFY volumeChanged);
    Q_PROPERTY( bool repeatState READ getRepeatState NOTIFY statusUpdated);
    Q_PROPERTY( int playerState READ getPlayerState NOTIFY statusUpdated);
    Q_PROPERTY( float epasedTime READ getElapsedTime NOTIFY statusUpdated);
    Q_PROPERTY( float duration READ getDuration NOTIFY statusUpdated);
    Q_PROPERTY( QString currentPlayedInfo READ getCurrentPlayedInfo NOTIFY fileInfoUpdated);

private:
    QString host;
    int port;
    QString password;
    Connector *client;
    QTimer *statusRenew, *sender;
    Dataparser *mpdData;

    QByteArray receivedData;
    QQueue<QByteArray> *request;

    int s_volume;
    bool s_repeat;
    bool s_random;
    bool s_single;
    bool s_consume;
    int s_playlist;
    int s_playlistLength;
    double s_mixRampDb;
    int s_state;
    int s_song;
    int s_songId;
    QString s_time;
    float s_elapsed;
    int s_bitrate;
    float s_duration;
    QString s_audioFormat;
    int s_nextSong;
    int s_nextSongId;

    QString f_songInfo;
    QString f_lastModified;
    int f_time;
    float f_duration;
    int f_position;
    int f_songId;


    enum State
    {
        Play,
        Pause,
        Stop,
    };

signals:
    void statusUpdated();
    void fileInfoUpdated();
    void volumeChanged();

public:
    MultimediaPlayer(QString mpdServerAddress, int mpdPort, QString mpdPassword);
    int getCurrentVolume() {return s_volume;}
    bool getRepeatState() {return s_repeat;}
    int getPlayerState() {return s_state;}
    float getElapsedTime() {return s_elapsed;}
    float getDuration() {return s_duration;}
    QString getCurrentPlayedInfo() {return f_songInfo;}
    //...

public slots:
    void stateInfo(QString data);
    void songInfo(QString data);

    void onReceive();
    void onGetStatus();
    void onSend();

    void volume(int value);
    void play();
    void stop();
    void pause();
    void next();
    void prev();
    void random(bool isTrue);
    void repeat(bool isTrue);
    void playSong(int songId);
    void removeSong( int songId);
    void clearQueue();
    void addPlaylist(QString name);
    void removePlaylist(QString playlist);
    void clearPlaylist(QString playlist);
    void addSongToPlaylist(QString playlist, QString songName);
    void playPlaylist(QString playlist);
    void listPlaylist(QString playlist);
    void seek(int time);
    void listDirectory(QString path);
    void listPlaylists();

    void sendCudtomCmd(QString command);


    /* DBG print*/
    void stateConsolePrint();
    void currentSongConsolePrint();


};

#endif // MULTIMEDIAPLAYER_H
