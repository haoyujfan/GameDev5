extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if GameManager.game_over:
		rpc("game_over")

@rpc("any_peer", "call_local")
func game_over():
	print("game over")
	print(multiplayer.get_unique_id())
	GameManager.Players.erase(multiplayer.get_unique_id())
	var players = get_tree().get_nodes_in_group("Players")
	for item in players :
		if item.name == str(multiplayer.get_unique_id()) :
			item.queue_free()
	queue_free()
	# disconnect signals 
	GameManager.readied = false
