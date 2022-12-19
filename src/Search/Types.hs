{-# LANGUAGE DeriveGeneric         #-}
{-# LANGUAGE DuplicateRecordFields #-}

module Search.Types where

import Data.Aeson (FromJSON(..), ToJSON(..), genericToEncoding, defaultOptions)
import Data.Text (Text)
import Database.SQLite.Simple (FromRow(..), field)
import GHC.Generics (Generic)

data Page = Page
    { url :: !Text
    , title :: !Text
    , body :: !Text
    } deriving (Show, Generic)

instance ToJSON Page where
    toEncoding = genericToEncoding defaultOptions
instance FromJSON Page

data Result = Result
    { url :: !Text
    , title :: !Text
    , result :: !Text
    } deriving (Show, Generic)

instance ToJSON Result where
    toEncoding = genericToEncoding defaultOptions
instance FromJSON Result

instance FromRow Result where
    fromRow = Result <$> field <*> field <*> field
