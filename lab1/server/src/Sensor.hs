module Sensor where

data Location = Location {
                    latitude  :: Double
                    longitude :: Double
}
                  

data Sensor = Sensor {
                username  :: String
                location  :: Location
                ipAddress :: String
                port      :: Int
}
