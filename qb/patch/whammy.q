// Whammy gives the same amount of star power regardless of framerate

script star_power_whammy 
	if ($<player_status>.star_power_used = 1)
		return
	endif
	last_x = 0
	last_y = 0
	dir_x = 1
	dir_y = 1
	first = 1
	whammy_on = 0
	whammy_star_on = 0
	whammy_star_off = 0
	ExtendCRC star_whammy_on <player_text> out = id
	BroadcastEvent type = <id> data = {pattern = <pattern> time = <time> guitar_stream = <guitar_stream> song = <song> array_entry = <array_entry> player = <player> player_status = <player_status> player_text = <player_text>}
	<boss> = 0
	if ($boss_battle = 1)
		if (($<player_status>.player) = 2)
			<boss> = 1
		endif
	endif
	<do_blue_whammys> = 1
	if ($game_mode = p2_battle || $boss_battle = 1)
		<do_blue_whammys> = 0
	endif
	begin
	if (($<player_status>.whammy_on) = 0)
		ExtendCRC star_whammy_off <player_text> out = id
		BroadcastEvent type = <id> data = {pattern = <pattern> time = <time> guitar_stream = <guitar_stream> song = <song> array_entry = <array_entry> player = <player> player_status = <player_status> player_text = <player_text> finished = 0}
		break
	endif
	if (<boss> = 0)
		if GuitarGetAnalogueInfo controller = ($<player_status>.controller)
			if IsGuitarController controller = ($<player_status>.controller)
				<px> = ((<rightx> - $<player_status>.resting_whammy_position) / (1.0 - $<player_status>.resting_whammy_position))
				if (<px> < 0)
					<px> = 0
				endif
				if (<first> = 1)
					<last_x> = <px>
					<first> = 0
				endif
				<xdiff> = (<px> - <last_x>)
				if (<xdiff> < 0)
					<xdiff> = (0.0 - <xdiff>)
				endif
				if (<xdiff> > 0.03)
					<whammy_on> = 5
				endif
			else
				<px> = 0
				<py> = 0
				if (<leftlength> > 0)
					<px> = <leftx>
					<py> = <lefty>
				else
					if (<rightlength> > 0)
						<px> = <rightx>
						<py> = <RightY>
					endif
				endif
				if (<first> = 1)
					<last_x> = <px>
					<last_y> = <py>
					<first> = 0
				endif
				<xdiff> = (<px> - <last_x>)
				if (<xdiff> < 0)
					<xdiff> = (0.0 - <xdiff>)
				endif
				<ydiff> = (<py> - <last_y>)
				if (<ydiff> < 0)
					<ydiff> = (0.0 - <ydiff>)
				endif
				if (<xdiff> > 0.03)
					<whammy_on> = 5
				endif
				if (<ydiff> > 0.03)
					<whammy_on> = 5
				endif
			endif
			if (<whammy_on> > 0)
				<whammy_star_off> = 0
				<whammy_star_on> = (<whammy_star_on> + 1)
				beat_time = ($<player_status>.playline_song_beat_time / 1000.0)
				beat_ratio = (0.016683350016683352 / <beat_time>)
				if ($game_mode = p2_career || $game_mode = p2_coop)
					increase_star_power amount = ($star_power_whammy_add_coop * <beat_ratio>) player_status = <player_status>
				else
					increase_star_power amount = ($star_power_whammy_add * <beat_ratio>) player_status = <player_status>
				endif
				whammy_on = (<whammy_on> - 1)
				if (<do_blue_whammys> = 1)
					if (<whammy_star_on> = 5)
						GetArraySize \{$gem_colors}
						gem_count = 0
						begin
						if ((<pattern> && $button_values [<gem_count>]) = $button_values [<gem_count>])
							FormatText checksumname = whammy_id '%c_%i_whammybar_p%p' c = ($gem_colors_text [<gem_count>]) i = <array_entry> p = ($<player_status>.player) AddToStringLookup = true
							if ScreenElementExists id = <whammy_id>
								bar_name = (<whammy_id> + 1)
								MakeStarWhammy name = <bar_name> player = ($<player_status>.player)
							endif
						endif
						gem_count = (<gem_count> + 1)
						repeat <array_size>
					endif
				endif
			else
				<whammy_star_on> = 0
				<whammy_star_off> = (<whammy_star_off> + 1)
				if (<do_blue_whammys> = 1)
					if (<whammy_star_off> = 5)
						GetArraySize \{$gem_colors}
						gem_count = 0
						begin
						if ((<pattern> && $button_values [<gem_count>]) = $button_values [<gem_count>])
							FormatText checksumname = whammy_id '%c_%i_whammybar_p%p' c = ($gem_colors_text [<gem_count>]) i = <array_entry> p = ($<player_status>.player) AddToStringLookup = true
							if ScreenElementExists id = <whammy_id>
								bar_name = (<whammy_id> + 1)
								MakeNormalWhammy name = <bar_name> player = ($<player_status>.player)
							endif
						endif
						gem_count = (<gem_count> + 1)
						repeat <array_size>
					endif
				endif
			endif
			<last_x> = <px>
			<last_y> = <py>
		endif
	endif
	<time> = (<time> - (0.016683350016683352 * 1000))
	if (<time> <= 0)
		ExtendCRC star_whammy_off <player_text> out = id
		BroadcastEvent type = <id> data = {pattern = <pattern> time = <time> guitar_stream = <guitar_stream> song = <song> array_entry = <array_entry> player = <player> player_status = <player_status> player_text = <player_text> finished = 1}
		break
	endif
	Wait \{16.683350016683352
		milliseconds}
	repeat
endscript