extends Node2D


@export var Address = "127.0.0.1"
@export var port = 8910
var peer
@onready var start_button = $Start

# Called when the node enters the scene tree for the first time.
func _ready():
	start_button.disabled = true
	multiplayer.peer_connected.connect(peer_connected)
	multiplayer.peer_disconnected.connect(peer_disconnected)
	multiplayer.connected_to_server.connect(connected_to_server)
	multiplayer.connection_failed.connect(connection_failed)
	pass # Replace with function body.

# called on the server and clients when they connect
func peer_connected(id):
	print("Player connected " + str(id))


# called on the server and clients when they disconnect
func peer_disconnected(id):
	print("Player disconnected "  + str(id))
	GameManager.Players.erase(id)
	var players = get_tree().get_nodes_in_group("Players")
	for item in players :
		if item.name == str(id) :
			item.queue_free()
	get_node("../main").queue_free()
	get_tree().change_scene_to_file("res://scenes/disconnect_win_screen.tscn")
	
# called on only the clients when they connect
func connected_to_server():
	print("Connected to Server!")
	SendPlayerInformation.rpc_id(1, $UsernameInput.text, multiplayer.get_unique_id())

# called on only the clients when they disconnect
func connection_failed():
	print("Couldn't connect")
	
@rpc("any_peer")
func SendPlayerInformation(name, id):
	if !GameManager.Players.has(id):
		GameManager.Players[id] = { 
			"name": name,
			"id" : id,
			"score" : 0
		}
	
	if multiplayer.is_server():
		for i in GameManager.Players:
			SendPlayerInformation.rpc(GameManager.Players[i].name, i)
			
	
func _on_host_button_down():
	peer = ENetMultiplayerPeer.new()
	var error = peer.create_server(port, 2) # 2 player game
	if error != OK:
		print("cannot host: " + error_string(error))
		return
	peer.get_host().compress(ENetConnection.COMPRESS_RANGE_CODER)
	
	multiplayer.set_multiplayer_peer(peer)
	print("Waiting for players!")
	SendPlayerInformation($UsernameInput.text, multiplayer.get_unique_id())
	
	start_button.disabled = false
	
func _on_join_button_down():
	peer = ENetMultiplayerPeer.new()
	peer.create_client(Address, port)
	peer.get_host().compress(ENetConnection.COMPRESS_RANGE_CODER)
	multiplayer.set_multiplayer_peer(peer)
	
	start_button.disabled = false

@rpc("any_peer", "call_local")
func start_game():
	var scene = load("res://scenes/main.tscn").instantiate()
	get_tree().root.add_child(scene)
	self.hide()
#	get_tree().change_scene_to_file("res://scenes/main.tscn")

func _on_start_button_down():
	start_game.rpc()
	
func _on_back_pressed():
	get_tree().change_scene_to_file("res://scenes/main_menu.tscn")
