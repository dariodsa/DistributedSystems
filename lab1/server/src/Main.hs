{-# LANGUAGE OverloadedStrings #-}
module Main where


import Web.Scotty
import Network.HTTP.Types

routes :: ScottyM ()
routes = do 
  get "/sensors" $ do 
    text "Sensori"
  post "/sensors" $ do 
    text "Dodajem"
  get "/sensors/:id" $ do 
    text "Specifikacije senzora"


--main :: IO ()
main = do 
   putStrLn "Starting Server..."
   scotty 3000 routes

