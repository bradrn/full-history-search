{-# LANGUAGE DisambiguateRecordFields #-}
{-# LANGUAGE LambdaCase               #-}
{-# LANGUAGE NamedFieldPuns           #-}
{-# LANGUAGE OverloadedStrings        #-}

module Search.Insert where

import Data.Text (Text)
import Database.SQLite.Simple

import Search.Types

executeInsert :: Connection -> (Text -> IO ()) -> Page -> IO ()
executeInsert conn log Page{url,title,body} = do
    fold conn
        "SELECT rowid,url FROM pages WHERE url MATCH ?" (Only $ toPrefixMatch url)
        Nothing ((pure .) . updateExistence url)
        >>= \case
            Nothing -> do -- this URL is new
                log $ "Inserting " <> url
                executeNamed conn
                    "INSERT INTO pages(url, title, body) VALUES(:url, :title, :body)"
                    [ ":url" := url
                    , ":title" := title
                    , ":body" := body
                    ]
            Just rowid -> do -- update existing row
                log $ "Updating " <> url
                executeNamed conn
                    "UPDATE pages SET title = :title, body = :body WHERE rowid = :rowid"
                    [ ":rowid" := rowid
                    , ":title" := title
                    , ":body" := body
                    ]
  where
    toPrefixMatch str = "^\"" <> str <> "\""

    updateExistence :: Text -> Maybe Int -> (Int, Text) -> Maybe Int
    updateExistence _ existence@(Just _) _ = existence
    updateExistence url' Nothing (rowid, rowurl) 
        | url' == rowurl = Just rowid
        | otherwise = Nothing
