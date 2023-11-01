extends Node3D

@export var PlayerScene: PackedScene

# Called when the node enters the scene tree for the first time.
func _ready():

	var index = 0
	for i in GameManager.Players:
		var currentPlayer = PlayerScene.instantiate()
		currentPlayer.set_multiplayer_authority(GameManager.Players[i].id)
		# currentPlayer.name = str(GameManager.Players[i].id) # can access in c++ with get_name()
		add_child(currentPlayer)
		for spawn in get_tree().get_nodes_in_group("SpawnPoint"):
			if spawn.name == "Spawn" + str(index):
				currentPlayer.global_position = spawn.global_position
		index += 1


