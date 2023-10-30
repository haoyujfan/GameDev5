extends Player

func _ready():
#	$MultiplayerSynchronizer.set_multiplayer_authority(str(name).to_int())
	$MultiplayerSynchronizer.set_multiplayer_authority(1)

