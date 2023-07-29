# -*- coding: utf-8 -*-

from odoo import  _
from odoo.models import MetaModel
import xw_cbase

xw_cbase.patch_meta_model({
    'MetaModel': MetaModel,
})
