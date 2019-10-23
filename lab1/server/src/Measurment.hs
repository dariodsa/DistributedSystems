module Measurment where
import Sensor

data Measurment = Measurment {
                    sensor    :: Sensor
                    parameter :: String
                    avgValue  :: Float
}
