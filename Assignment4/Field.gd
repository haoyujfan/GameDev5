extends Node3D

@export var PlayerScene: PackedScene
var Effects : RichTextLabel
var Rotate : RichTextLabel
var Music : RichTextLabel
var Player1 : Player
var Player2 : Player

# Called when the node enters the scene tree for the first time.
func _ready():
	var index = 0
	for i in GameManager.Players:
		var currentPlayer = PlayerScene.instantiate()
		currentPlayer.set_multiplayer_authority(GameManager.Players[i].id)
		currentPlayer.name = str(GameManager.Players[i].id) # can access in c++ with get_name()
		add_child(currentPlayer)
		if (i == 1):
			Player1 = currentPlayer
		else:
			Player2 = currentPlayer
		for spawn in get_tree().get_nodes_in_group("SpawnPoint"):
			if spawn.name == "Spawn" + str(index):
				currentPlayer.global_position = spawn.global_position
		index += 1
	
	if GameManager.Players.size() == 2 :
		for j in GameManager.Players:
			if (j == 1):
				Player2.set_other_id(GameManager.Players[j].id)
			else:
				Player1.set_other_id(GameManager.Players[j].id)

func _process(delta):
	if GameManager.new_client_connected and multiplayer.get_unique_id() == 1 :
		var index = 0
		for i in GameManager.Players:
			if i != 1 :
				var currentPlayer = PlayerScene.instantiate()
				currentPlayer.set_multiplayer_authority(GameManager.Players[i].id)
				currentPlayer.name = str(GameManager.Players[i].id) # can access in c++ with get_name()
				add_child(currentPlayer)
				Player2 = currentPlayer
				for spawn in get_tree().get_nodes_in_group("SpawnPoint"):
					if spawn.name == "Spawn" + str(index):
						currentPlayer.global_position = spawn.global_position
				index += 1
				
				for j in GameManager.Players:
					if (j == 1):
						Player2.set_other_id(GameManager.Players[j].id)
					else:
						Player1.set_other_id(GameManager.Players[j].id)
		GameManager.new_client_connected = false
#	if GameManager.mode == "multiplayer":
#		$Lives.position = Vector2(20, 600)
#		$Lives.text = "Player 1 Lives: " + str(Player1.get_lives())
#		if GameManager.Players.size() > 1 and Player2:
#			$Lives2.position = Vector2(883, 600)
#			$Lives2.text = "Player 2 Lives: " + str(Player2.get_lives())
#	else:
#		$Lives.text = "Lives: " + str(Player1.get_lives())
#
#	if (Player1 and Player1.get_game_over()) || (Player2 and Player2.get_game_over()):
#		GameManager.game_over = true
#		self.hide()

#func _on_player_sound_effect_toggle(toggle: String):
#	Effects.text = "Sound Effect Mute: " + toggle
#
#func _on_player_rotate_mode_toggle(toggle: String):
#	Rotate.text = "Rotate Mode: R " + toggle
#
#func _on_ground_music_toggle(toggle: String):
#	Music.text = "Music Mute: M " + toggle
