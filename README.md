mqtt_tiny_clock_pub.rb
====
publish the local HHMM time using MQTT.

setup
----
    $ sudo gem install mqtt
    $ sudo gem install pit

how to use
----
    $ git https://github.com/yoggy/mqtt_tiny_clock_pub.git
    $ cd mqtt_clock_pub
    $ EDITOR=vim ruby mqtt_clock_pub.rb
    
        ---
        remote_host: mqtt.example.com
        remote_port: 1883
        use_auth: false
        username: username
        password: password
        topic: publish_topic

    connected!

        .
        .
        .
        .

