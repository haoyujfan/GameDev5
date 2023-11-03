extends Node3D

@export var PlayerScene: PackedScene
var Lives : RichTextLabel
var Effects : RichTextLabel
var Rotate : RichTextLabel
var Music : RichTextLabel
var PointsValue : int
var numPlayers : int
var Player1 : Player
var Player2 : Player

# Called when the node enters the scene tree for the first time.
func _ready():
	numPlayers = 1
	PointsValue = 0
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
			numPlayers = 2
		for spawn in get_tree().get_nodes_in_group("SpawnPoint"):
			if spawn.name == "Spawn" + str(index):
				currentPlayer.global_position = spawn.global_position
		index += 1

func _process(delta):
	# Points.Text = "Points: " + get_node<Player>("Player").get_points() # If Points is a RichTextLabel
	if (numPlayers == 2):
		$Lives.text = "Player 1 Lives: " + str(Player1.get_lives())
		$Lives2.position = Vector2(883, 600)
		$Lives2.text = "Player 2 Lives: " + str(Player2.get_lives())
	
	else:
		$Lives.text = "Lives: " + str(Player1.get_lives())
#
#func _on_player_interact_orange():
#	PointsValue += 1
#
#func _lose_life():
#	PointsValue -= 1
#
#func _on_player_life_lost_attacker():
#	PointsValue -= 1

#func _on_player_sound_effect_toggle(toggle: String):
#	Effects.text = "Sound Effect Mute: " + toggle
#
#func _on_player_rotate_mode_toggle(toggle: String):
#	Rotate.text = "Rotate Mode: R " + toggle
#
#func _on_ground_music_toggle(toggle: String):
#	Music.text = "Music Mute: M " + toggle
