#include "multimediaplayer.h"
#include <QTcpSocket>

MultimediaPlayer::MultimediaPlayer(QString mpdServerAddress, int mpdPort, QString mpdPassword)
{
    host = mpdServerAddress;
    port = mpdPort;
    password = mpdPassword;

    client = new Connector(host, port);
    connect(client, SIGNAL(connected()), this, SLOT(onReceive()));
    connect(client, SIGNAL(newData()), this, SLOT(onReceive()));

    statusRenew = new QTimer(this);
    connect(statusRenew, SIGNAL(timeout()), this, SLOT(onGetStatus()));
    statusRenew->setInterval(100);
    statusRenew->start();

    sender = new QTimer(this);
    connect(sender, SIGNAL(timeout()), this, SLOT(onSend()));
    sender->setInterval(1000);
    sender->start();

    request = new QQueue<QByteArray>();
}

void MultimediaPlayer::stateInfo(QString data)
{
    QHash<QString, QString> dataToParse = Dataparser::parse(data);

    s_volume = dataToParse.value(VOLUME_CMD, DEFAULT_NEG_VAL).toInt();
    s_repeat = dataToParse.value(REPEAT_CMD) == DEFAULT_TRUE;
    s_random = dataToParse.value(RANDOM_CMD) == DEFAULT_TRUE;
    s_single = dataToParse.value(SINGLE_CMD) == DEFAULT_TRUE;
    s_consume = dataToParse.value(CONSUME_CMD) == DEFAULT_TRUE;
    s_playlist = dataToParse.value(PLAYLIST_CMD, DEFAULT_NEG_VAL).toInt();
    s_playlistLength = dataToParse.value(PLAYLIST_LEN_CMD, DEFAULT_VAL).toInt();
    s_mixRampDb = dataToParse.value(MIX_RAMP_DB_CMD, DEFAULT_VAL).toDouble();

    QString tempState = dataToParse.value(STATE_CMD);
    if(tempState == STOP_CMD)
        s_state = Stop;
    if(tempState == PLAY_CMD)
        s_state = Play;
    s_state = Stop;
    if(tempState == PAUSE_CMD)
        s_state = Pause;

    s_song = dataToParse.value(SONG_CMD, DEFAULT_NEG_VAL).toInt();
    s_songId = dataToParse.value(SONG_ID_CMD, DEFAULT_NEG_VAL).toInt();
    s_time = dataToParse.value(TIME_CMD);
    s_elapsed = dataToParse.value(ELAPSED_CMD, DEFAULT_NEG_VAL).toFloat();
    s_bitrate = dataToParse.value(BITRATE_CMD, DEFAULT_NEG_VAL).toInt();
    s_duration = dataToParse.value(DURATION_CMD, DEFAULT_NEG_VAL).toFloat();
    s_audioFormat = dataToParse.value(AUDIO_FORMAT_CMD);
    s_nextSong = dataToParse.value(NEXT_SONG_CMD, DEFAULT_NEG_VAL).toInt();
    s_nextSongId = dataToParse.value(NEXT_SONG_ID_CMD, DEFAULT_NEG_VAL).toInt();

    emit statusUpdated();
    emit volumeChanged();
}

void MultimediaPlayer::songInfo(QString data)
{
    QHash<QString, QString> dataToParse = Dataparser::parse(data);

    f_songInfo = dataToParse.value(FILE_CMD);
    f_lastModified = dataToParse.value(LAST_MOD_CMD);
    f_time = dataToParse.value(TIME_CMD, DEFAULT_NEG_VAL).toInt();
    f_duration = dataToParse.value(DURATION_CMD, DEFAULT_NEG_VAL).toFloat();
    f_position = dataToParse.value(POSITION_CMD,DEFAULT_NEG_VAL).toInt();
    f_songId = dataToParse.value(ID_CMD,DEFAULT_NEG_VAL).toInt();

    emit fileInfoUpdated();
}

void MultimediaPlayer::onReceive()
{    
    receivedData = client->read();
    stateInfo(receivedData);
    songInfo(receivedData);

    //qDebug() << "RECV: " + receivedData;
    //stateConsolePrint(); // DBG
    //currentSongConsolePrint(); // DBG
}

void MultimediaPlayer::onGetStatus()
{
    request->enqueue(Dataparser::makeRequest(STATUS_REQUEST));
    request->enqueue(Dataparser::makeRequest(CURRENT_FILE_INFO_REQUEST)); // todo fix
}

void MultimediaPlayer::onSend()
{
    while(!request->isEmpty())
        client->write(request->dequeue());

}

void MultimediaPlayer::volume(int value)
{
    request->enqueue(mpdData->makeReguestWithValue(SET_VOLUME_CMD, value));
}
void MultimediaPlayer::play()
{
    request->enqueue(Dataparser::makeRequest(PLAY_CMD));
}
void MultimediaPlayer::stop()
{
    request->enqueue(Dataparser::makeRequest(STOP_CMD));
}
void MultimediaPlayer::pause()
{
    request->enqueue(Dataparser::makeRequest(PAUSE_CMD));
}
void MultimediaPlayer::next()
{
    request->enqueue(Dataparser::makeRequest(NEXT_CMD));
}
void MultimediaPlayer::prev()
{
    request->enqueue(Dataparser::makeRequest(PREV_CMD));
}
void MultimediaPlayer::random(bool isTrue)
{
    request->enqueue((Dataparser::makeReguestWithValue(RANDOM_CMD, isTrue)));
}
void MultimediaPlayer::repeat(bool isTrue)
{
    request->enqueue((Dataparser::makeReguestWithValue(REPEAT_CMD, isTrue)));
}
void MultimediaPlayer::playSong(int songId)
{
    request->enqueue((Dataparser::makeReguestWithValue(SONG_ID_CMD, songId)));
}
void MultimediaPlayer::removeSong( int songId)
{
    // todo
}
void MultimediaPlayer::clearQueue()
{
    //todo
}
void MultimediaPlayer::addPlaylist(QString name)
{
    request->enqueue((Dataparser::makeReguestWithValue(PLAYLIST_SAVE_CMD, name)));
}
void MultimediaPlayer::removePlaylist(QString playlist)
{
    request->enqueue((Dataparser::makeReguestWithValue(PLAYLIST_RMV_CMD, playlist)));
}

void MultimediaPlayer::clearPlaylist(QString playlist)
{
    request->enqueue(Dataparser::makeReguestWithValue(PLAYLIST_CLEAR_CMD, playlist));
}

void MultimediaPlayer::addSongToPlaylist(QString playlist, QString songName)
{
    request->enqueue(Dataparser::makeReguestWithEscapedValue(PLAYLIST_ADD_CMD, playlist, songName));
}
void MultimediaPlayer::playPlaylist(QString playlist)
{
    request->enqueue(Dataparser::makeRequest((QString(PLAY_PLAYLIST_MACRO_CMD).arg(playlist)).toUtf8()));
}

void MultimediaPlayer::listPlaylist(QString playlist)
{
    request->enqueue(Dataparser::makeReguestWithValue(PLAYLIST_LIST_CMD, playlist));
}

void MultimediaPlayer::seek(int time)
{
    request->enqueue((Dataparser::makeReguestWithValue(SEEK_CMD, s_song, time)));
}
void MultimediaPlayer::listDirectory(QString path)
{
    request->enqueue(Dataparser::makeReguestWithValue(LIST_ALL_IN_DIRECTORY_CMD, path));
}

void MultimediaPlayer::listPlaylists()
{
    request->enqueue(Dataparser::makeRequest("listplaylists"));
}

/* DBG print */

void MultimediaPlayer::sendCudtomCmd(QString command)
{
    request->enqueue(Dataparser::makeRequest(command.toUtf8()));
}

void MultimediaPlayer::stateConsolePrint()
{
/*
"RECV:
volume: 33\n
repeat: 1\n
random: 1\n
single: 0\n
consume: 0\n
playlist: 4\n
playlistlength: 3\n
mixrampdb: 0.000000\n
state: play\n
song: 0\n
songid: 1\n
time: 34:442\n
elapsed: 33.978\n
bitrate: 256\n
duration: 442.320\n
audio: 48000:24:2\n
nextsong: 1\n
nextsongid: 2\nOK\n"
*/

    qDebug() << "VOL: " << s_volume << " REP: " << s_repeat << " RAND: " << s_random
             << " SINGLE: " << s_single << " CONSUME: " << s_consume << " Num on Playlist: " << s_playlist
             << " PLAYLIST LEN: " << s_playlistLength << " MIX RAMP: " << s_mixRampDb
             << " STATE: " << s_state << " SONG: " << s_song << " SONGID: "<< s_songId
             << " TIME: "  << s_time << " ELAPS: " << s_elapsed << " BITRATE: " << s_bitrate
             << " DURATION: " << s_duration << " FORMAT: " << s_audioFormat << " NEXT: "
             << s_nextSong << " NEXTID: " << s_nextSongId;
}

void MultimediaPlayer::currentSongConsolePrint()
{
/*
"RECV:
file: Dr Alban 'No Coke' (12 'mix).mp3\n
Last-Modified: 2022-03-04T01:05:58Z\n
Time: 442\n
duration: 442.320\n
Pos: 0\n
Id: 1\n
OK\n"
*/
    qDebug() << "File name: " << f_songInfo << " Modified: " << f_lastModified
             << " Time: " << f_time
             << " Duration: " << f_duration << " Position: " << f_position
             << " Song ID: " << f_songId;
}
