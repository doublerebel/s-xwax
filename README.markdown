**s-xwax: *super-xwax* -- an experimental fork of xwax Digital Vinyl System**

The goal of s-xwax is to widen the interest and use of xwax by adding
attractive and popular modern DVS features, including the most popular xwax
community mods and flashy UI updates, showing that a F/OSS DVS can be
feature-complete, stable, and efficient enough for regular pro DJ use.

Built from [the latest xwax beta](http://www.xwax.co.uk/devel/xwax.git)

  * Original source by Mark Hills

  * Deck info UI by [Sighter/JVA](http://sighter.ath.cx/cgit/cgit.cgi/xwaxed/?h=played_tracks)

  * Deck protection patch by [Yves Adler](https://github.com/yadler/xwax-yadler)

  * Played/Loaded tracks crate and indicator by [Yves Adler](https://github.com/yadler/xwax-yadler)

  * Stacked waveform, overlay, and album art UI by Charles Phillips

  * XSPF and PLS playlist scanner by Charles Phillips

[Screenshot of s-xwax 0.1a](http://www.doublerebel.com/images/s-xwax0.1a.png)

*Video tearing due to lack of vsync in SDL 1.2, upgrading to 1.3 is in the TODO*

New album art feature reads **folder.jpg** from the folder containing
the audio file.  The script **albumart** can be used to recursively
create the **folder.jpg** files (requires **Imagemagick** to be installed).

New **XSPF**/**PLS** playlist scanner will read playlists and the track
metadata encoded in the playlists.  Playlists can be created by
programs such as **Audacious**, **Rhythmbox**, and many more. Make sure
that the playlist editor reads track metadata on import. Note: This feature
uses the -l command line option to load playlists, completely separate from
Mark's new -p option.

To increase performance, reduce the size of the waveform display
by reducing the PLAYER_HEIGHT constant in **interface.c** .

**Many many thanks** to Mark Hills and the other xwax contributors.
I am only building small changes on top of a fantastic program.
This is my first C project in almost a decade, patches and bug
reports are gladly accepted.

WARNING: Consider this repository to be unstable and volatile.
S-xwax has not been fully tested under performance conditions.
Large waveforms have a significant negative impact on performance.
(I will be rewriting the UI functions to use OpenGL acceleration.)
This branch is subject to change.

**Original README:**

xwax: Digital vinyl on Linux

(C) Copyright 2011 Mark Hills <mark@pogo.org.uk>

For installation instructions, see the INSTALL file. Instructions can
be found in the xwax(1) man page and http://www.xwax.co.uk/

xwax is vinyl emulation software for Linux. It allows DJs and
turntablists to playback digital audio files (MP3, Ogg Vorbis, FLAC,
AAC and more), controlled using a normal pair of turntables via
timecoded vinyls.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2, as
published by the Free Software Foundation.
 
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License version 2 for more details.
 
You should have received a copy of the GNU General Public License
version 2 along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
