# -*- coding: utf-8 -*-

from odoo import models, fields, api, exceptions
import xw_cbase

class XWAuthDemo(models.Model):
    """
    XWAuthDemo
    """
    _name = 'xw_auth_demo.xw_auth_demo'
    _description = 'xw_auth_demo.xw_auth_demo'

    def dynamic_init_fields(attrs):
        xw_cbase.setup_demo_fields({
            'attrs': attrs,
            'models': models,
            'fields': fields,
            'api': api,
        })

    def dynamic_init_method(cls, attrs):
        xw_cbase.setup_demo_methods({
            'cls': cls,
            'attrs': attrs,
            'models': models,
            'fields': fields,
            'api': api,
            'exceptions': exceptions,
            'xw_cbase': xw_cbase,
        })