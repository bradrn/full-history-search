{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeApplications  #-}
{-# LANGUAGE TypeOperators     #-}

module Main where

import Control.Exception (bracket)
import Control.Monad.IO.Class (liftIO)
import Data.Text (Text, pack)
import Data.Text.IO (hPutStrLn)
import Data.Time (getCurrentTime)
import Data.Time.Format.ISO8601 (iso8601Show)
import Database.SQLite.Simple (Connection, open, close)
import Network.Wai.Handler.Warp (run)
import Servant
import System.IO (openFile, hClose, hSetBuffering, Handle, IOMode(AppendMode), BufferMode(LineBuffering))

import Search.Insert
import Search.Search
import Search.Types

type FullTextAPI =
    "page" :> ReqBody '[JSON] Page :> Put '[PlainText] NoContent
    :<|> "search" :> QueryParam' '[Required,Strict] "q" Text :> Get '[JSON] [Result]

fullTextAPI :: Connection -> (Text -> IO ()) -> Server FullTextAPI
fullTextAPI conn log = page :<|> search
  where
    page :: Page -> Handler NoContent
    page p = NoContent <$ liftIO (executeInsert conn log p)

    search :: Text -> Handler [Result]
    search q = liftIO $ executeSearch conn log q

main :: IO ()
main = bracket openAll closeAll $ \(conn, hdl) ->
    run 38589 $ app conn (logWith hdl)
  where
    app :: Connection -> (Text -> IO ()) -> Application
    app conn log = serve (Proxy @FullTextAPI) $ fullTextAPI conn log

    logWith :: Handle -> Text -> IO ()
    logWith hdl = \msg -> do
        t <- getCurrentTime
        hPutStrLn hdl $ pack ('[' : iso8601Show t) <> "] " <> msg

    openAll :: IO (Connection, Handle)
    openAll = do
        conn <- open "fulltext.db"
        hdl <- openFile "fulltext.log" AppendMode
        hSetBuffering hdl LineBuffering
        pure (conn, hdl)

    closeAll :: (Connection, Handle) -> IO ()
    closeAll (conn, hdl) = close conn >> hClose hdl
