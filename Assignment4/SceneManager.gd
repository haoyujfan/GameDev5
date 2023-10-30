extends Node3D

@export var PlayerScene: PackedScene
var player1 : Player
var player2 : Player
var spawn0 : Node3D
var spawn1 : Node3D
# Called when the node enters the scene tree for the first time.
func _ready():
	player1 = get_node("Player")
	player2 = get_node("Player2")
	for i in GameManager.Players:
		print(GameManager.Players[i].id)

#	player1.name = str(GameManager.Players[1].id)
	#player2.name = str(GameManager.Players[1].id)
	spawn0 = get_node("Spawn0")
	spawn1 = get_node("Spawn1")
	player1.global_position = spawn0.global_position
	player2.global_position = spawn1.global_position
#	for i in GameManager.Players:
#		if i != 1:
#			//player2.name = str(GameManager.Players[i].id)
#			print(GameManager.Players[i].id)
#   this doesn't currently work, we want it to work later
#	var index = 0
#	for i in GameManager.Players:
#		var currentPlayer = PlayerScene.instantiate()
#		add_child(currentPlayer)
#		for spawn in get_tree().get_nodes_in_group("SpawnPoint"):
#			if spawn.name == "Spawn" + str(index):
#				currentPlayer.global_position = spawn.global_position
#			index += 1
#	pass # Replace with function body.

